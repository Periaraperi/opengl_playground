#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "matrix.hpp"

class Input_Manager;
namespace peria::graphics {

class Camera {
public:
    Camera() = default;
    Camera(const glm::vec3& pos_, const glm::vec3& target_pos_, const glm::vec3& up_);

    void update(const Input_Manager* const im);
    void update_pos(const glm::vec3& p);
    void update_camera_front(float mouse_delta_x, float mouse_delta_y);

    [[nodiscard]]
    const glm::mat4& get_view() const;

    [[nodiscard]]
    glm::vec3 get_pos() const;

    [[nodiscard]]
    glm::vec3 get_view_direction() const;

    [[nodiscard]]
    const Matrix4& get_peria_view() const;

private:
    glm::vec3 pos;
    glm::vec3 target_pos;
    glm::vec3 up;
    glm::vec3 cam_front;

    float pitch {};
    float yaw {-90.0f};

    glm::mat4 view;
    Matrix4 peria_view;
};
}
