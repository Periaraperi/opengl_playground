#include "sampler.hpp"
#include "simple_logger.hpp"
#include <glad/glad.h>

#include <utility>
#include <array>

namespace peria::graphics {

Sampler::Sampler()
{ 
    peria::log("Creating Sampler object");
    glCreateSamplers(1, &id); 

    glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// REFACTOR THIS SHIT LATER
Sampler::Sampler(u8 dummy_val)
{ 
    peria::log("Creating Sampler object");
    glCreateSamplers(1, &id); 
    if (dummy_val == 0) {
        glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else if (dummy_val == 1) {
        glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else if (dummy_val == 2) {
        glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        std::array border_color {1.0f, 1.0f, 1.0f, 1.0f};
        glSamplerParameterfv(id, GL_TEXTURE_BORDER_COLOR, border_color.data());
    }
    else if (dummy_val == 3) {
        glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    else { // temporarily use this for skybox cubemaps
        glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
}

Sampler::~Sampler()
{ 
    peria::log("Destroying Sampler object");
    glDeleteSamplers(1, &id);
}

Sampler::Sampler(Sampler&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{ peria::log("Move constructing Sampler"); }

Sampler& Sampler::operator=(Sampler&& rhs) noexcept
{
    if (&rhs == this) return *this;
    peria::log("Move assigning Sampler"); 

    this->id = std::exchange(rhs.id, 0);
    return *this;
}

void Sampler::bind(u32 unit) const noexcept
{ glBindSampler(unit, id); }

u32 Sampler::sampler_id() const noexcept
{ return id; }

}
