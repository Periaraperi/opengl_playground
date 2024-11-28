#include "input_manager.hpp"

#include <SDL2/SDL.h>
#include <algorithm> // for std::copy

#include "simple_logger.hpp"

Input_Manager::Input_Manager()
    :keyboard_state{nullptr}, 
    prev_keyboard_state{nullptr}, 
    key_length{0}
{
    // no need to call SDL_GetKeyboardState again, we pollevents in game loop which
    // pumps events, which updates keyboard_state array, we just maintain pointer to it
    keyboard_state = SDL_GetKeyboardState(&key_length);
    prev_keyboard_state = new u8[key_length];
    std::copy(keyboard_state, keyboard_state+key_length, prev_keyboard_state);
    mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    
    peria::log("Input Manager ctor()");
}

Input_Manager::~Input_Manager()
{
    peria::log("Input Manager dtor()");
    delete[] prev_keyboard_state; 
    prev_keyboard_state = nullptr;
}

void Input_Manager::update_prev_state()
{
    std::copy(keyboard_state, keyboard_state+key_length, prev_keyboard_state);
    prev_mouse_state = mouse_state;
}

void Input_Manager::update_mouse()
{ mouse_state = SDL_GetMouseState(&mouse_x,&mouse_y); }

std::pair<i32, i32> Input_Manager::get_mouse()
{ return {mouse_x, mouse_y}; }

u32 Input_Manager::get_mask(Mouse_Button btn)
{
    u32 mask {};
    switch (btn) {
        case Mouse_Button::LEFT:
            mask = SDL_BUTTON_LMASK;
            break;
        case Mouse_Button::MID:
            mask = SDL_BUTTON_MMASK;
            break;
        case Mouse_Button::RIGHT:
            mask = SDL_BUTTON_RMASK;
            break;
    }
    return mask;
}

bool Input_Manager::key_pressed(SDL_Scancode key)
{ return (keyboard_state[key] && !prev_keyboard_state[key]); }

bool Input_Manager::key_down(SDL_Scancode key)
{ return (keyboard_state[key] && prev_keyboard_state[key]); }

bool Input_Manager::key_released(SDL_Scancode key)
{ return (!keyboard_state[key] && prev_keyboard_state[key]); }

bool Input_Manager::mouse_pressed(Mouse_Button btn)
{
    auto mask {get_mask(btn)};
    return ((mouse_state&mask)!=0 && (prev_mouse_state&mask)==0);
}

bool Input_Manager::mouse_down(Mouse_Button btn)
{
    auto mask {get_mask(btn)};
    return ((mouse_state&mask)!=0 && (prev_mouse_state&mask)!=0);
}

bool Input_Manager::mouse_released(Mouse_Button btn)
{
    auto mask {get_mask(btn)};
    return ((mouse_state&mask)==0 && (prev_mouse_state&mask)!=0);
}
