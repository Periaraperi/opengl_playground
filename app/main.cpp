#include "peria_engine/peria_engine.hpp"
#include <iostream>

class Test : public peria::Application {
public:
    Test()
    {
        // init stuff here
        // setup graphics rendering modes, like batching and stuff ...
    }
    
    void update() override
    {
    }

    void render() override
    {
        draw_rect(peria::graphics::NAVY);
    }
private:
    //Members needed for project
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try {
        Test app{};
        app.run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
