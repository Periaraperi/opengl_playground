#include "app.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    peria::App_Settings settings {"xD", 1600, 900, true};

    peria::App app {std::move(settings)};
    if (!app.is_initialized()) {
        return EXIT_FAILURE;
    }

    app.run();

    return EXIT_SUCCESS;
}
