#include "sampler.hpp"
#include "simple_logger.hpp"
#include <glad/glad.h>

#include <utility>

namespace peria {

Sampler::Sampler()
{ peria::log("Sampler ctor()"); glCreateSamplers(1, &id); }

Sampler::~Sampler()
{ peria::log("Sampler dtor()"); glDeleteSamplers(1, &id); }

Sampler::Sampler(Sampler&& rhs) noexcept
    :id{std::exchange(rhs.id, 0)}
{}

Sampler& Sampler::operator=(Sampler&& rhs) noexcept
{
    if (&rhs == this) return *this;
    this->id = std::exchange(rhs.id, 0);
    return *this;
}

void setup_sampler_parameters(u32 sampler_id, u32 min_filter, u32 mag_filter, u32 wrap_s, u32 wrap_t, u32 wrap_r) noexcept
{
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, min_filter);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, mag_filter);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, wrap_s);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, wrap_t);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_R, wrap_r);
}

}

