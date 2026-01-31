#include "cam2d.hpp"
#include "graphics.hpp"
#include "input_manager.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace peria {

glm::vec2 Camera2D::screen_to_world(const glm::vec2& p, const glm::mat4& projection)
{
    const auto screen_dims {get_screen_dimensions()};
    // w is in range -1 1 so clip and ndc is same I guess.
    const auto clip {glm::vec2{2.0f*p.x / screen_dims.x - 1.0f, // from ndc to clip
                               2.0f*p.y / screen_dims.y - 1.0f}};
    const auto world {glm::inverse(projection*view)*glm::vec4{clip.x, clip.y, 0.0f, 1.0f}};
    //const auto world {glm::inverse(view)*view_space};
    return {world.x, world.y};
}

void Camera2D::update(const glm::mat4& projection)
{
    auto im {Input_Manager::instance()};
    glm::vec2 mouse_screen {};
    {
        auto m {im->get_mouse()};
        m.second = get_screen_dimensions().y - m.second;
        mouse_screen = {m.first, m.second};
    }

    if (im->mouse_down(Mouse_Button::LEFT) && get_mouse_moved()) {
        const auto rel {get_relative_motion()};
        pos -= glm::vec2{rel.x, rel.y}/zoom_scale;
    }
    const auto mouse_before_zoom_world {screen_to_world(mouse_screen, projection)};
    bool zoomed {};
    if (im->key_pressed(SDL_SCANCODE_1)) {
        zoom_scale /= 2;
        zoomed = true;
    }
    if (im->key_pressed(SDL_SCANCODE_2)) {
        zoom_scale *= 2;
        zoomed = true;
    }
    if (zoomed) {
        // intermediate update to get view with new zoom
        view = glm::mat4{glm::scale(glm::mat4{1.0f}, {zoom_scale, zoom_scale, 1.0f})*
                         glm::translate(glm::mat4{1.0f}, {-pos, 0.0f})};
        const auto mouse_after_zoom_world {screen_to_world(mouse_screen, projection)};
        pos += (mouse_before_zoom_world-mouse_after_zoom_world);
    }

    // update view because of position change.
    view = glm::mat4{glm::scale(glm::mat4{1.0f}, {zoom_scale, zoom_scale, 1.0f})*
                     glm::translate(glm::mat4{1.0f}, {-pos, 0.0f})};
}

}
