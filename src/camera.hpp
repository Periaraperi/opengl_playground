#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "matrix.hpp"

namespace peria::graphics {
class Camera {
public:
    Camera() = default;
    Camera(const glm::vec3& pos_, const glm::vec3& target_pos_, const glm::vec3& up_);
    void update(const glm::vec3& dir);
    void update_pos(const glm::vec3& p);

    [[nodiscard]]
    const glm::mat4& get_view();

    [[nodiscard]]
    const Matrix4& get_peria_view();

private:
    glm::vec3 pos;
    glm::vec3 target_pos;
    glm::vec3 up;

    glm::mat4 view;
    Matrix4 peria_view;
};
}
