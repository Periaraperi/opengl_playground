#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "logger.hpp"

namespace peria::engine {
Graphics::Graphics()
{
    PERIA_LOG("Graphics Initialized");
}

Graphics::~Graphics()
{
    PERIA_LOG("Graphics dtor()");
}
}
