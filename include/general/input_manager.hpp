#pragma once

#include <cstdint>
#include <SDL_scancode.h>
#include <glm/vec2.hpp>

enum class Mouse_Button {
    LEFT,
    MID,
    RIGHT
};

class Input_Manager {
public:
    static void shutdown(); 
    static Input_Manager& get();

    bool key_pressed(SDL_Scancode key);
    bool key_held(SDL_Scancode key);
    bool key_released(SDL_Scancode key);
    
    bool mouse_pressed(Mouse_Button btn);
    bool mouse_held(Mouse_Button btn);
    bool mouse_released(Mouse_Button btn);

    glm::vec2 get_mouse_pos();

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
    int _mouse_x, _mouse_y;
};
