#include "matrix.hpp"
#include <cmath>

#include <glm/geometric.hpp>

namespace peria::graphics {

Matrix4::Matrix4() noexcept
{
    this->operator()(0, 0) = 1.0f;
    this->operator()(1, 1) = 1.0f;
    this->operator()(2, 2) = 1.0f;
    this->operator()(3, 3) = 1.0f;
}

Matrix4::Matrix4(float x) noexcept
{
    for (auto& i:mat) {
        i = x;
    }
}

float& Matrix4::operator()(std::size_t r, std::size_t c) noexcept
{ return mat[r*4 + c]; }

Matrix4 Matrix4::operator*(const Matrix4& rhs) noexcept
{
    Matrix4 res{0.0f};
    for (std::size_t r{}; r<4; ++r) {
        for (std::size_t c{}; c<4; ++c) {
            for (std::size_t k{}; k<4; ++k) {
                res(r, c) += this->operator()(r, k) * rhs.mat[k*4 + c];
            }
        }
    }
    return res;
}

const float* Matrix4::get_data() const noexcept
{ return mat.data(); }

Matrix4 get_ortho_projection(float left, float right, float bottom, float top) noexcept
{
    Matrix4 m{0.0f};
    m(0, 0) = 2.0f / (right - left);
    m(0, 1) = 0.0f;
    m(0, 2) = 0.0f;
    m(0, 3) = -(right + left) / (right - left);
                                                
    m(1, 0) = 0.0f;
    m(1, 1) = 2.0f / (top - bottom);
    m(1, 2) = 0.0f;
    m(1, 3) = -(top + bottom) / (top - bottom);
                                                
    m(2, 0) = 0.0f;
    m(2, 1) = 0.0f;
    m(2, 2) = 1.0f;
    m(2, 3) = 0.0f;
                   
    m(3, 0) = 0.0f;
    m(3, 1) = 0.0f;
    m(3, 2) = 0.0f;
    m(3, 3) = 1.0f;
    return m;
}

Matrix4 get_ortho_projection(float left, float right, float bottom, float top, float near, float far) noexcept
{
    Matrix4 m{0.0f};
    m(0, 0) = 2.0f / (right - left);
    m(0, 1) = 0.0f;
    m(0, 2) = 0.0f;
    m(0, 3) = -(right + left) / (right - left);
                                                
    m(1, 0) = 0.0f;
    m(1, 1) = 2.0f / (top - bottom);
    m(1, 2) = 0.0f;
    m(1, 3) = -(top + bottom) / (top - bottom);
                                                
    m(2, 0) = 0.0f;
    m(2, 1) = 0.0f;                                                     
    m(2, 2) = -2.0f / (far - near);                                                     
    m(2, 3) = -(far + near) / (far - near);                             
                                                
    m(3, 0) = 0.0f;
    m(3, 1) = 0.0f;
    m(3, 2) = 0.0f;
    m(3, 3) = 1.0f;
    return m;
}

Matrix4 get_perspective_projection(float left, float right, float bottom, float top, float near, float far) noexcept
{
    Matrix4 m{0.0f};
    m(0, 0) = (2.0f * near) / (right - left);
    m(0, 1) = 0.0f;
    m(0, 2) = (right + left) / (right - left);
    m(0, 3) = 0.0f;
                                                
    m(1, 0) = 0.0f;
    m(1, 1) = (2.0f * near) / (top - bottom);
    m(1, 2) = (top + bottom) / (top - bottom);
    m(1, 3) = 0.0f;
                                                
    m(2, 0) = 0.0f;
    m(2, 1) = 0.0f;                                                     
    m(2, 2) = -(far + near) / (far - near);                             
    m(2, 3) = (-2.0f * far * near) / (far - near);                                                     
                                                
    m(3, 0) = 0.0f;
    m(3, 1) = 0.0f;
    m(3, 2) = -1.0f;
    m(3, 3) = 0.0f;
    return m;
}

Matrix4 get_perspective_projection(float fov_y, float aspect_ratio, float near, float far) noexcept
{
    Matrix4 m{0.0f};

    const auto top = near * std::tanf(fov_y*0.5f);
    const auto right = aspect_ratio * top;

    m(0, 0) = near / right;
    m(0, 1) = 0.0f;
    m(0, 2) = 0.0f;
    m(0, 3) = 0.0f;
                                                
    m(1, 0) = 0.0f;
    m(1, 1) = near / top;
    m(1, 2) = 0.0f;
    m(1, 3) = 0.0f;
                                                
    m(2, 0) = 0.0f;
    m(2, 1) = 0.0f;                                                     
    m(2, 2) = -(far + near) / (far - near);                             
    m(2, 3) = (-2.0f * far * near) / (far - near);                                                     
                                                
    m(3, 0) = 0.0f;
    m(3, 1) = 0.0f;
    m(3, 2) = -1.0f;
    m(3, 3) = 0.0f;
    return m;
}

Matrix4 get_look_at(const glm::vec3& view, const glm::vec3& target, const glm::vec3& up) noexcept
{
    Matrix4 m{}; // local axes info
    Matrix4 n{}; // translation info

    n(0, 3) = -view.x;
    n(1, 3) = -view.y;
    n(2, 3) = -view.z;

    const auto camera_local_z {glm::normalize(view - target)};
    const auto camera_local_x {glm::normalize(glm::cross(up, camera_local_z))};
    const auto camera_local_y {glm::cross(camera_local_z, camera_local_x)};

    // [ camera_local_x.x, camera_local_x.y, camera_local_x.z, 0.0f ] 
    // [ camera_local_y.x, camera_local_y.y, camera_local_y.z, 0.0f ] 
    // [ camera_local_z.x, camera_local_z.y, camera_local_z.z, 0.0f ] 
    // [ 0.0f,             0.0f,             0.0f,             1.0f ] 

    m(0, 0) = camera_local_x.x;
    m(0, 1) = camera_local_x.y;
    m(0, 2) = camera_local_x.z;

    m(1, 0) = camera_local_y.x;
    m(1, 1) = camera_local_y.y;
    m(1, 2) = camera_local_y.z;

    m(2, 0) = camera_local_z.x;
    m(2, 1) = camera_local_z.y;
    m(2, 2) = camera_local_z.z;
    
    return m*n;
}

Matrix4 translate(float x, float y, float z) noexcept
{
    Matrix4 m{};
    m(0, 3) = x;
    m(1, 3) = y;
    m(2, 3) = z;
    return m;
}

Matrix4 scale(float x, float y, float z) noexcept
{
    Matrix4 m{};
    m(0, 0) = x;
    m(1, 1) = y;
    m(2, 2) = z;
    return m;
}

Matrix4 rotate(float angle_x /*in radians*/, float angle_y, float angle_z) noexcept
{
    Matrix4 m{};
    if (angle_x != 0.0f) {
        Matrix4 rot_x{};
        rot_x(1, 1) = std::cos(angle_x);
        rot_x(1, 2) = -std::sin(angle_x);
        rot_x(2, 1) = std::sin(angle_x);
        rot_x(2, 2) = std::cos(angle_x);
        m = rot_x * m;
    }
    if (angle_y != 0.0f) {
        Matrix4 rot_y{};
        rot_y(0, 0) = std::cos(angle_y);
        rot_y(0, 2) = std::sin(angle_y);
        rot_y(2, 0) = -std::sin(angle_y);
        rot_y(2, 2) = std::cos(angle_y);
        m = rot_y * m;
    }
    if (angle_z != 0.0f) {
        Matrix4 rot_z{};
        rot_z(0, 0) = std::cos(angle_z);
        rot_z(0, 1) = -std::sin(angle_z);
        rot_z(1, 0) = std::sin(angle_z);
        rot_z(1, 1) = std::cos(angle_z);
        m = rot_z * m;
    }

    return m;
}

}


