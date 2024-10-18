#include "engine.hpp"

#include <iostream>

class sandbox : public peria::engine::application {
public:
    explicit sandbox(peria::engine::window_settings settings_)
        :application{std::move(settings_)}
    { std::cerr << "sandbox initializing\n"; }

    ~sandbox() override
    { std::cerr << "sandbox shutting down\n"; }

    void update() override
    { 
        if (input->key_pressed(peria::engine::PERIA_KEY_SPACE)) {
            graphics->set_clear_color(peria::color::AQUA);
        }
        if (input->key_pressed(peria::engine::PERIA_KEY_RETURN)) {
            graphics->set_clear_color(peria::color::WHITE);
        }
    }

    void render() override
    { }
private:
};

int main()
{
    try {
        auto app = std::make_unique<sandbox>(peria::engine::window_settings{"sandbox", 800, 600, true});
        app->run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
