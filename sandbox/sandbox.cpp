#include "engine.hpp"

class Sandbox : public peria::engine::Application {
public:
    explicit Sandbox(const peria::engine::Window_Settings& settings)
        :Application{settings}
    { PERIA_LOG("Sandbox Constructor!"); }

    ~Sandbox() override
    { PERIA_LOG("Sandbox Destructor!"); }

    void update() override
    {
        if (_input_manager->key_pressed(peria::engine::Peria_Key::PERIA_KEY_RETURN)) {
            std::cerr << "FUCK\n";
        }
    }

    void render() override
    {
    }
private:
};

int main()
{
    try {
        auto sandbox = std::make_unique<Sandbox>(peria::engine::Window_Settings{"sandbox", 800, 600, false, true});
        sandbox->run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
