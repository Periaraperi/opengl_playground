#pragma once

#include "peria_types.hpp"
#include <memory>

namespace peria {

class Application {
public:
    Application();
    Application(const char* title, i32 window_width, i32 window_height, bool resizable);

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) noexcept;
    Application& operator=(Application&&) noexcept;

    ~Application();

    void run();

private:
    struct App_Impl;
    std::unique_ptr<App_Impl> app_impl;
};
}
