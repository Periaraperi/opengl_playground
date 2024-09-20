#pragma once

#include <string>
#include "peria_types.hpp"

namespace peria::engine {
struct Window_Settings {
    std::string title{"Application"};
    i32 width{800};
    i32 height{600};
    bool resizable{false};
    bool opengl{false};
};
}
