#include <engine/platform/win32/win32_gl_context.hpp>

#include <cstdio>
#include <system_error>

namespace engine::platform {
    bool Win32GLContext::init() {
        h_DeviceCtx = GetDC(h_Window);

        PIXELFORMATDESCRIPTOR pfd{};

        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;

        int pixelFormat = ChoosePixelFormat(h_DeviceCtx, &pfd);
        if (!SetPixelFormat(h_DeviceCtx, pixelFormat, &pfd)) {
            printf("SetPixelFormat failed: %s\n", std::system_category().message(GetLastError()).c_str());
            return false;
        }

        h_GlCtx = wglCreateContext(h_DeviceCtx);
        if (!h_GlCtx) {
            printf("wglCreateContext failed: %s\n", std::system_category().message(GetLastError()).c_str());
            destroy();
            return false;
        }

        return true;
    }

    void Win32GLContext::use() {
        if (h_Window != nullptr && h_GlCtx) {
            wglMakeCurrent(h_DeviceCtx, h_GlCtx);
        }
    }

    void Win32GLContext::discard() {
        wglMakeCurrent(nullptr, nullptr);
    }

    void Win32GLContext::destroy() {
        if (h_GlCtx) {
            wglDeleteContext(h_GlCtx);
            h_GlCtx = nullptr;
        }

        if (h_Window && h_DeviceCtx) {
            ReleaseDC(h_Window, h_DeviceCtx);
            h_DeviceCtx = nullptr;
        }
    }

    void Win32GLContext::render() {
        if (h_DeviceCtx) {
            wglSwapLayerBuffers(h_DeviceCtx, WGL_SWAP_MAIN_PLANE);
        }
    }
}