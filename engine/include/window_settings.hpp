#pragma once

#include <string>
#include "peria_types.hpp"

namespace peria::engine {
struct window_settings {
    std::string title{"Application"};
    i32 width{800};
    i32 height{600};
    bool resizable{false};
};
}
