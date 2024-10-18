#pragma once

#include "peria_types.hpp"
#include <vector>

namespace peria::engine {
class vertex_array {
public:
    struct vertex_attribute {
        i32 count;
        u32 type;
        bool normalized; 
        std::size_t stride;
    };

    vertex_array() = default;
    explicit vertex_array(std::vector<vertex_attribute>&& vertex_attributes);
    ~vertex_array();

    vertex_array(const vertex_array&) = delete;
    vertex_array& operator=(const vertex_array&) = delete;
    vertex_array(vertex_array&&) noexcept = default;
    vertex_array& operator=(vertex_array&&) noexcept = default;

private:
    u32 id;

    std::vector<vertex_attribute> attributes;
};

}
