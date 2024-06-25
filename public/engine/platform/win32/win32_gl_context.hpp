#pragma once

#include <Windows.h>

#include <engine/core/graphics/i_graphics_context.hpp>

namespace engine::platform {
    struct Win32GLContext : public core::IGraphicsContext {
        Win32GLContext(HWND win) : h_Window{win} {}

        bool init() override;

        void use() override;

        void discard() override;

        void destroy() override;

        void render() override;

    protected:
        HGLRC h_GlCtx;
        HWND h_Window;
        HDC h_DeviceCtx;
    };
}