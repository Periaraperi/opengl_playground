#pragma once

#include "peria_types.hpp"
#include <glm/vec2.hpp>

#include "peria_input.hpp"

namespace peria::engine {

class Input_Manager {
public: // functions
    Input_Manager();
    ~Input_Manager();

    [[nodiscard]]
    bool key_pressed(Peria_Key key);

    [[nodiscard]]
    bool key_down(Peria_Key key);

    [[nodiscard]]
    bool key_released(Peria_Key key);
 
    [[nodiscard]]
    bool mouse_pressed(Mouse_Button btn);

    [[nodiscard]]
    bool mouse_down(Mouse_Button btn);

    [[nodiscard]]
    bool mouse_released(Mouse_Button btn);

    [[nodiscard]]
    Mouse get_mouse();

    void update_prev_state();
    void update_mouse();
private: // functions

    [[nodiscard]]
    u32 get_mask(Mouse_Button btn);
private: // members
    const u8* _keyboard_state;
    u8* _prev_keyboard_state;
    i32 _key_length;

    u32 _mouse_state;
    u32 _prev_mouse_state;
    i32 _mouse_x, _mouse_y;

public: // disable copying and moving
    Input_Manager(const Input_Manager&) = delete;
    Input_Manager operator=(const Input_Manager&) = delete;
    Input_Manager(Input_Manager&&) = delete;
    Input_Manager operator=(Input_Manager&&) = delete;
};
}
