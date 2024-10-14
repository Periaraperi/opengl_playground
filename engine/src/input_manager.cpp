#include "input_manager.hpp"

#include <SDL2/SDL.h>
#include <algorithm> // for std::copy
#include <iostream>

namespace peria::engine {

input_manager::input_manager()
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
    
    std::cerr << "Input Manager initialized\n";
}

input_manager::~input_manager()
{
    delete[] prev_keyboard_state; 
    prev_keyboard_state = nullptr;
    std::cerr << "Input Manager shutting down\n";
}

void input_manager::update_prev_state()
{
    std::copy(keyboard_state, keyboard_state+key_length, prev_keyboard_state);
    prev_mouse_state = mouse_state;
}

void input_manager::update_mouse()
{ mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y); }

peria::engine::mouse input_manager::get_mouse()
{ return {mouse_x, mouse_y}; }

u32 input_manager::get_mask(mouse_button btn)
{
    u32 mask = 0;
    switch (btn) {
        case mouse_button::LEFT:
            mask = SDL_BUTTON_LMASK;
            break;
        case mouse_button::MID:
            mask = SDL_BUTTON_MMASK;
            break;
        case mouse_button::RIGHT:
            mask = SDL_BUTTON_RMASK;
            break;
    }
    return mask;
}

bool input_manager::key_pressed(peria_key key)
{ return (keyboard_state[key] && !prev_keyboard_state[key]); }

bool input_manager::key_down(peria_key key)
{ return (keyboard_state[key] && prev_keyboard_state[key]); }

bool input_manager::key_released(peria_key key)
{ return (!keyboard_state[key] && prev_keyboard_state[key]); }

bool input_manager::mouse_pressed(mouse_button btn)
{
    auto mask = get_mask(btn);
    return ((mouse_state&mask)!=0 && (prev_mouse_state&mask)==0);
}

bool input_manager::mouse_down(mouse_button btn)
{
    auto mask = get_mask(btn);
    return ((mouse_state&mask)!=0 && (prev_mouse_state&mask)!=0);
}

bool input_manager::mouse_released(mouse_button btn)
{
    auto mask = get_mask(btn);
    return ((mouse_state&mask)==0 && (prev_mouse_state&mask)!=0);
}
}
