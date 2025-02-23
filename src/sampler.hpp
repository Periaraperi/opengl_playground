#pragma once

#include "peria_types.hpp"

namespace peria {

struct Sampler {
    Sampler();
    ~Sampler();

    Sampler(const Sampler&) = delete;
    Sampler& operator=(const Sampler&) = delete;

    Sampler(Sampler&&) noexcept;
    Sampler& operator=(Sampler&&) noexcept;

    u32 id;
};

}
