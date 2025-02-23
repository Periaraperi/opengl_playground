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

}
