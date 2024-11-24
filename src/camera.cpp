#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace peria::graphics {

Camera::Camera(const glm::vec3& pos_, const glm::vec3& target_pos_, const glm::vec3& up_)
    :pos{pos_},
     target_pos{target_pos_},
     up{up_},
     view{glm::lookAt(pos, target_pos, up_)}
{}

void Camera::update(const glm::vec3& p, const glm::vec3& dir)
{
    //static auto t {0.00f};
    //const auto radius {10.0f};
    //auto cam_x {std::sin(t) * radius};
    //auto cam_z {std::cos(t) * radius};
    //view = glm::lookAt(glm::vec3(cam_x, 0.0, cam_z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  
    //t += 0.003f;
    pos += p;
    view = glm::lookAt(pos, pos+dir, up);
}

const glm::mat4& Camera::get_view()
{ return view; }

}
