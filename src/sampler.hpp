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

//class Sampler {
//public:
//    Sampler();
//
//    Sampler(const Sampler&) = delete;
//    Sampler& operator=(const Sampler&) = delete;
//
//    Sampler(Sampler&&) noexcept;
//    Sampler& operator=(Sampler&&) noexcept;
//
//    ~Sampler();
//
//    void bind(u32 unit = 0) const noexcept;
//
//    [[nodiscard]]
//    u32 sampler_id() const noexcept;
//private:
//    u32 id;
//};
}
