#include "camera.hpp"
#include "input_manager.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace peria {

Camera::Camera(const glm::vec3& pos_, const glm::vec3& target_pos_, const glm::vec3& up_)
    :pos{pos_},
     target_pos{target_pos_},
     up{up_},
     cam_front{0.0f, 0.0f, -1.0f},
     view{glm::lookAt(pos, target_pos, up)},
     peria_view{get_look_at(pos, target_pos, up)}
{}

void Camera::update()
{
    auto im {Input_Manager::instance()};

    float speed {0.05f};
    if (im->key_down(SDL_SCANCODE_LSHIFT)) {
        speed *= 2.0f;
    }
    if (im->key_down(SDL_SCANCODE_W)) {
        update_pos(cam_front*speed);
    }
    if (im->key_down(SDL_SCANCODE_S)) {
        update_pos(-cam_front*speed);
    }
    if (im->key_down(SDL_SCANCODE_D)) {
        update_pos(glm::normalize(glm::cross(cam_front, up))*speed);
    }
    if (im->key_down(SDL_SCANCODE_A)) {
        update_pos(-glm::normalize(glm::cross(cam_front, up))*speed);
    }

    const auto target {pos+cam_front};
    view = glm::lookAt(pos, target, up);
    peria_view = get_look_at(pos, target, up);
}

void Camera::update_camera_front(float mouse_delta_x, float mouse_delta_y)
{
    const auto sensitivity {0.05f}; // hardcode for now
    yaw += mouse_delta_x * sensitivity;
    pitch += mouse_delta_y * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    
    cam_front = glm::normalize(front);
}

void Camera::update_pos(const glm::vec3& p)
{ pos += p; }

glm::vec3 Camera::get_pos() const
{ return pos; }

const glm::mat4& Camera::get_view() const
{ return view; }

const Matrix4& Camera::get_peria_view() const
{ return peria_view; }

glm::vec3 Camera::get_view_direction() const
{ return cam_front; }

void Camera::set_view_direction(glm::vec3 view_dir)
{ cam_front = view_dir; }

}
