#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Input_Manager;
namespace peria {

struct Camera2D {
    Camera2D() = default;

    void update(const glm::mat4& projection);

    glm::vec2 pos {0.0f, 0.0f};
    float zoom_scale {1.0f};
    glm::mat4 view {1.0f};
private:
    [[nodiscard]]
    glm::vec2 screen_to_world(const glm::vec2& p, const glm::mat4& projection);
};

}
