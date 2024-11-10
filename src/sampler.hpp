#pragma once

#include "peria_types.hpp"

namespace peria::graphics {
class Sampler {
public:
    explicit Sampler(u8 dummy_val = 0); // TODO: change this later. We do this for testing

    Sampler(const Sampler&) = delete;
    Sampler& operator=(const Sampler&) = delete;

    Sampler(Sampler&&) noexcept = default;
    Sampler& operator=(Sampler&&) noexcept = default;

    ~Sampler();

    void bind(u32 unit = 0) const noexcept;

    [[nodiscard]]
    u32 sampler_id() const noexcept;
private:
    u32 id;
};
}
