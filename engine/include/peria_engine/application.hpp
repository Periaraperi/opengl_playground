#pragma once

#include <memory>
#include "peria_types.hpp"
#include "peria_color.hpp"

namespace peria {

class Application {
public:
    Application();
    Application(const char* title, i32 window_width, i32 window_height, bool resizable);

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) noexcept;
    Application& operator=(Application&&) noexcept;

    virtual ~Application();

    void run();

    virtual void update() = 0;
    virtual void render() = 0;

    void draw_rect(const graphics::Color<float>& color);

private:
    struct App_Impl;
    std::unique_ptr<App_Impl> app_impl;
};
}
