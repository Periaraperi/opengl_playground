#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace peria::graphics {

Camera::Camera(const glm::vec3& pos_, const glm::vec3& target_pos_, const glm::vec3& up_)
    :pos{pos_},
     target_pos{target_pos_},
     up{up_},
     view{glm::lookAt(pos, target_pos, up)},
     peria_view{get_look_at(pos, target_pos, up)}
{}

void Camera::update(const glm::vec3& dir)
{
    view = glm::lookAt(pos, pos+dir, up);
    peria_view = get_look_at(pos, pos+dir, up);
}

void Camera::update_pos(const glm::vec3& p)
{ pos += p; }

glm::vec3 Camera::get_pos()
{ return pos; }

const glm::mat4& Camera::get_view()
{ return view; }

const Matrix4& Camera::get_peria_view()
{ return peria_view; }

}
