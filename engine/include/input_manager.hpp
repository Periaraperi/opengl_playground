#pragma once

#include "peria_types.hpp"
#include <glm/vec2.hpp>

#include "peria_input.hpp"

namespace peria::engine {

class input_manager {
public:
    input_manager();
    ~input_manager();

    [[nodiscard]]
    bool key_pressed(peria_key key);

    [[nodiscard]]
    bool key_down(peria_key key);

    [[nodiscard]]
    bool key_released(peria_key key);
 
    [[nodiscard]]
    bool mouse_pressed(mouse_button btn);

    [[nodiscard]]
    bool mouse_down(mouse_button btn);

    [[nodiscard]]
    bool mouse_released(mouse_button btn);

    [[nodiscard]]
    mouse get_mouse();

    void update_prev_state();
    void update_mouse();
private:

    [[nodiscard]]
    u32 get_mask(mouse_button btn);
private:
    const u8* keyboard_state;
    u8* prev_keyboard_state;
    i32 key_length;

    u32 mouse_state;
    u32 prev_mouse_state;
    i32 mouse_x, mouse_y;

public:
    input_manager(const input_manager&) = delete;
    input_manager operator=(const input_manager&) = delete;
    input_manager(input_manager&&) = default;
    input_manager& operator=(input_manager&&) = default;
};
}
