#include "sampler.hpp"
#include "simple_logger.hpp"
#include <glad/glad.h>

namespace peria::graphics {

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
    else {
        glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
}

Sampler::~Sampler()
{ 
    peria::log("Destroying Sampler object");
    glDeleteSamplers(1, &id);
}

void Sampler::bind(u32 unit) const noexcept
{ glBindSampler(unit, id); }

u32 Sampler::sampler_id() const noexcept
{ return id; }

}
