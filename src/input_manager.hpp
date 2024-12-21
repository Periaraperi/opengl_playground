#pragma once

#include <utility>
#include <SDL2/SDL_scancode.h>

#include "peria_types.hpp"

enum class Mouse_Button {
    LEFT = 0,
    MID,
    RIGHT
};

class Input_Manager {
public:
    Input_Manager();
    ~Input_Manager();

    bool key_pressed(SDL_Scancode key) const;
    bool key_down(SDL_Scancode key) const;
    bool key_released(SDL_Scancode key) const;
 
    bool mouse_pressed(Mouse_Button btn) const;
    bool mouse_down(Mouse_Button btn) const;
    bool mouse_released(Mouse_Button btn) const;

    std::pair<i32, i32> get_mouse() const;

    void update_prev_state();
    void update_mouse();
private:

    uint32_t get_mask(Mouse_Button btn) const;
private:

    const u8* keyboard_state;
    u8* prev_keyboard_state;
    i32 key_length;

    u32 mouse_state;
    u32 prev_mouse_state;
    i32 mouse_x, mouse_y;

public:
    // disable copying and moving
    Input_Manager(const Input_Manager&) = delete;
    Input_Manager operator=(const Input_Manager&) = delete;
    Input_Manager(Input_Manager&&) = delete;
    Input_Manager operator=(Input_Manager&&) = delete;
};
