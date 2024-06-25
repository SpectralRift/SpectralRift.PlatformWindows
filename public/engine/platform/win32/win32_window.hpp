#pragma once

#include <windows.h>
#include <string>
#include <stdexcept>

#include <engine/core/utils/vector2.hpp>
#include <engine/core/window/i_window.hpp>

namespace engine::platform {
    struct Win32Window : public core::IWindow {
        Win32Window() : h_Window(nullptr) {}

        bool init() override;

        void destroy() override;

        void setTitle(std::string_view title) override;

        void setPosition(core::utils::IVector2 position) override;

        void setSize(core::utils::IVector2 size) override;

        core::utils::IVector2 getPosition() override;

        core::utils::IVector2 getSize() override;

        void processEvents() override;

        void show() override;

        void hide() override;

        HWND getHandle() const {
            return h_Window;
        }

    protected:
        HWND h_Window;

        static bool b_IsWinClassRegistered;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}