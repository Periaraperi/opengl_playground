#pragma once

#include <array>
#include <ostream>

namespace peria::graphics {

class Matrix4 {
public:
    Matrix4() noexcept;
    explicit Matrix4(float x) noexcept;
    float& operator()(std::size_t r, std::size_t c) noexcept;

    Matrix4 operator*(const Matrix4& rhs) noexcept;

    [[nodiscard]]
    const float* get_data() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, Matrix4 rhs)
    {
        for (std::size_t r{}; r<4; ++r) {
            for (std::size_t c{}; c<4; ++c) {
                os << rhs.mat[4*r + c] << " ";
            }
            os << '\n';
        }
        return os;
    }
private:
    std::array<float, 16> mat{};
};

[[nodiscard]]
Matrix4 get_ortho_projection(float left, float right, float bottom, float top) noexcept;

[[nodiscard]]
Matrix4 get_ortho_projection(float left, float right, float bottom, float top, float near, float far) noexcept;

[[nodiscard]]
Matrix4 get_perspective_projection(float left, float right, float bottom, float top, float near, float far) noexcept;

[[nodiscard]]
Matrix4 translate(float x, float y, float z) noexcept;

[[nodiscard]]
Matrix4 scale(float x, float y, float z) noexcept;

[[nodiscard]]
Matrix4 rotate(float x, float y, float z) noexcept;

}
