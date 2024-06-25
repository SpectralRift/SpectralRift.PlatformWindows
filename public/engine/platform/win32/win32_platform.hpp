#pragma once

#include <engine/core/platform/i_platform.hpp>

namespace engine::platform {
    class Win32Platform : public core::IPlatform {
    public:
        Win32Platform() = default;

//            std::vector<DisplayInfo> getAvailableDisplays() override;
//            DisplayInfo getPrimaryDisplay() override;
        std::string getName() const override {
            return "win32";
        }

        void init() override {}

        void showMessageBox(std::string_view title, std::string_view message) override;
    };
}
