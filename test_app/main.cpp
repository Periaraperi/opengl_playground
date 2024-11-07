#include "application.hpp"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try {
        peria::Application app{};
        app.run();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
