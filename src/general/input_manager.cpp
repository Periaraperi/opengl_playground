#include "input_manager.hpp"

#include "logger.hpp"

#include <SDL2/SDL.h>
#include <stdlib.h>

Input_Manager* Input_Manager::_instance = nullptr;

Input_Manager& Input_Manager::get()
{
    if (_instance==nullptr) {
        _instance = new Input_Manager();
    }
    PERIA_ASSERT(_instance!=nullptr);
    return *_instance;
}

void Input_Manager::shutdown()
{
    delete _instance;
}

Input_Manager::Input_Manager()
    :_keyboard_state(nullptr), 
    _prev_keyboard_state(nullptr)
{
    _keyboard_state = SDL_GetKeyboardState(&_keyboard_len);
    PERIA_ASSERT(_keyboard_len!=0);
    _prev_keyboard_state = new uint8_t[_keyboard_len];
    memcpy(_prev_keyboard_state,_keyboard_state,_keyboard_len);
    _mouse_state = SDL_GetMouseState(&_mouse_x,&_mouse_y);
}

Input_Manager::~Input_Manager()
{
    delete[] _prev_keyboard_state;
    _prev_keyboard_state = nullptr;
}

void Input_Manager::update_prev_input_state()
{
    memcpy(_prev_keyboard_state,_keyboard_state,_keyboard_len);
    _prev_mouse_state = _mouse_state;
}

void Input_Manager::update_mouse()
{
    _mouse_state = SDL_GetMouseState(&_mouse_x,&_mouse_y);
}

bool Input_Manager::key_pressed(SDL_Scancode key)
{
    return (_keyboard_state[key] && !_prev_keyboard_state[key]);
}

bool Input_Manager::key_held(SDL_Scancode key)
{
    return (_keyboard_state[key] && _prev_keyboard_state[key]);
}

bool Input_Manager::key_released(SDL_Scancode key)
{
    return (!_keyboard_state[key] && _prev_keyboard_state[key]);
}

uint32_t Input_Manager::get_mask(Mouse_Button btn)
{
    uint32_t mask = 0;
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

bool Input_Manager::mouse_pressed(Mouse_Button btn)
{
    auto mask = get_mask(btn);
    return ((_mouse_state&mask)!=0 && (_prev_mouse_state&mask)==0);
}

bool Input_Manager::mouse_held(Mouse_Button btn)
{
    auto mask = get_mask(btn);
    return ((_mouse_state&mask)!=0 && (_prev_mouse_state&mask)!=0);
}

bool Input_Manager::mouse_released(Mouse_Button btn)
{
    auto mask = get_mask(btn);
    return ((_mouse_state&mask)==0 && (_prev_mouse_state&mask)!=0);
}

glm::vec2 Input_Manager::get_mouse_pos()
{
    return {_mouse_x,_mouse_y};
}

