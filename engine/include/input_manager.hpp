#pragma once

#include <cstdint>
#include <glm/vec2.hpp>

#include "peria_input.hpp"

namespace Peria_Engine {
class Input_Manager {
public:
    static void shutdown(); 
    static Input_Manager* get();

    bool key_pressed(Peria_Key key);
    bool key_held(Peria_Key key);
    bool key_released(Peria_Key key);
    
    bool mouse_pressed(Mouse_Button btn);
    bool mouse_held(Mouse_Button btn);
    bool mouse_released(Mouse_Button btn);

    glm::vec2 get_mouse_pos(); // by default returns mouse pos with SDL coord system

    void update_prev_input_state();
    void update_mouse();

private:
    Input_Manager(const Input_Manager&) = delete;
    Input_Manager(Input_Manager&&) = delete;
    Input_Manager& operator=(const Input_Manager&) = delete;
    Input_Manager& operator=(Input_Manager&&) = delete;
    
    Input_Manager();
    ~Input_Manager();

    uint32_t get_mask(Mouse_Button btn);

    static Input_Manager* _instance;
    int _keyboard_len;
    const uint8_t* _keyboard_state;
    uint8_t* _prev_keyboard_state;
    uint32_t _mouse_state;
    uint32_t _prev_mouse_state;
    int _mouse_x, _mouse_y; // mouse coordinates in window with SDL coordinate system
};
}
