#include "sampler.hpp"
#include "simple_logger.hpp"
#include <glad/glad.h>

#include <utility>

namespace peria {

Sampler::Sampler()
{ peria::log("Sampler ctor()"); glCreateSamplers(1, &id); }

Sampler::~Sampler()
{ peria::log("Sampler dtor()"); glDeleteSamplers(1, &id); id = 0; }

Sampler::Sampler(Sampler&& rhs) noexcept
{
    id = rhs.id;
    rhs.id = 0;
}

Sampler& Sampler::operator=(Sampler&& rhs) noexcept
{
    if (&rhs == this) return *this;
    std::swap(id, rhs.id);
    return *this;
}

}
