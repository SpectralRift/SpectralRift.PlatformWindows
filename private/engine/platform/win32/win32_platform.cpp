#include <engine/platform/win32/win32_platform.hpp>

#include <engine/core/platform/platform.hpp>
#include <engine/core/utils/static_factory_registrar.hpp>

#include <Windows.h>

namespace engine {
    namespace platform {
        core::utils::StaticFactoryRegistrar<core::Platform> s_Win32Registrar("win32",
                                                                               []() {
                                                                                   return std::make_unique<engine::platform::Win32Platform>();
                                                                               });

        void Win32Platform::showMessageBox(std::string_view title, std::string_view message) {
            MessageBoxA(nullptr, message.data(), title.data(), MB_OK);
        }
    }
}