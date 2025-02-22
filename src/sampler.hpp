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

void setup_sampler_parameters(u32 sampler_id, u32 min_filter, u32 mag_filter, u32 wrap_s, u32 wrap_t, u32 wrap_r) noexcept;

}
