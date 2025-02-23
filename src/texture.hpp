#pragma once

#include "peria_types.hpp"

namespace peria {

struct Texture2D {
    Texture2D() noexcept;

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& rhs) noexcept;
    Texture2D& operator=(Texture2D&& rhs) noexcept;

    ~Texture2D();

    u32 id;
};

struct Cubemap {
    Cubemap() noexcept;

    Cubemap(const Cubemap&) = delete;
    Cubemap& operator=(const Cubemap&) = delete;

    Cubemap(Cubemap&& rhs) noexcept;
    Cubemap& operator=(Cubemap&& rhs) noexcept;

    ~Cubemap();

    u32 id;
};

}
