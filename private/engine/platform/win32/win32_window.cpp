#include <engine/platform/win32/win32_window.hpp>
#include <engine/platform/win32/win32_gl_context.hpp>

#include <engine/core/window/window.hpp>
#include <engine/core/graphics/graphics_backend.hpp>
#include <engine/core/utils/static_factory_registrar.hpp>

namespace engine {
    namespace platform {
        bool Win32Window::b_IsWinClassRegistered{false};

        core::utils::StaticFactoryRegistrar<core::Window> s_Win32WindowRegistrar("win32",
                                                                                 []() {
                                                                                     return std::make_unique<Win32Window>();
                                                                                 });

        bool Win32Window::init() {
            const char *className = "SpectralRiftWin32";

            if (!b_IsWinClassRegistered) {
                WNDCLASS wc = {};
                wc.lpfnWndProc = WindowProc;
                wc.hInstance = GetModuleHandle(NULL);
                wc.lpszClassName = className;

                RegisterClass(&wc);
            }

            h_Window = CreateWindowEx(
                    0,
                    className,
                    "EngineWindow",
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                    nullptr,
                    nullptr,
                    GetModuleHandle(NULL),
                    this
            );

            if (h_Window == nullptr) {
                return false;
            }

            // set window user pointer to this
            SetWindowLongA(h_Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

            // decide what graphical context to use depending on preferred backend.
            auto backendName = core::GraphicsBackend::get()->getIdentifierName();

            if (backendName == "opengl") {
                m_GfxContext = std::make_unique<Win32GLContext>(h_Window);
            } else {
                printf("Unsupported backend: %s\n", backendName.data());
                return false;
            }

            if (!m_GfxContext->init()) {
                printf("Failed to create the graphical context!\n");
                return false;
            }

            return true;
        }

        void Win32Window::destroy() {
            if (m_GfxContext) {
                m_GfxContext->destroy();
                m_GfxContext = nullptr;
            }

            if (h_Window) {
                DestroyWindow(h_Window);
                h_Window = nullptr;
            }
        }

        void Win32Window::setTitle(std::string_view title) {
            if (h_Window) {
                SetWindowText(h_Window, title.data());
            }
        }

        void Win32Window::setPosition(core::utils::IVector2 position) {
            if (h_Window) {
                SetWindowPos(h_Window, nullptr, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
        }

        void Win32Window::setSize(core::utils::IVector2 size) {
            if (h_Window) {
                SetWindowPos(h_Window, nullptr, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOZORDER);
            }
        }

        core::utils::IVector2 Win32Window::getPosition() {
            RECT rect = {};
            if (h_Window && GetWindowRect(h_Window, &rect)) {
                return {rect.left, rect.top};
            }
            return {};
        }

        core::utils::IVector2 Win32Window::getSize() {
            RECT rect = {};
            if (h_Window && GetClientRect(h_Window, &rect)) {
                return {rect.right - rect.left, rect.bottom - rect.top};
            }
            return {};
        }

        void Win32Window::processEvents() {
            MSG msg = {};

            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    b_HasRequiredQuit = true;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        void Win32Window::show() {
            if (h_Window) {
                ShowWindow(h_Window, SW_SHOW);
            }
        }

        void Win32Window::hide() {
            if (h_Window) {
                ShowWindow(h_Window, SW_HIDE);
            }
        }

        LRESULT CALLBACK Win32Window::WindowProc(HWND h_Window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
            Win32Window *window = reinterpret_cast<Win32Window *>(GetWindowLongPtr(h_Window, GWLP_USERDATA));

            if (window) {
                switch (uMsg) {
                    case WM_DESTROY:
                        PostQuitMessage(0);
                        return 0;
                }
            }

            return DefWindowProc(h_Window, uMsg, wParam, lParam);
        }
    }
}