#pragma once

#include <utility>
#include <SDL3/SDL_scancode.h>

#include "peria_types.hpp"

enum class Mouse_Button {
    LEFT = 0,
    MID,
    RIGHT
};

class Input_Manager {
public:
    Input_Manager(const Input_Manager&) = delete;
    Input_Manager operator=(const Input_Manager&) = delete;

    Input_Manager(Input_Manager&&) = delete;
    Input_Manager operator=(Input_Manager&&) = delete;

    static void initialize() noexcept;
    static void shutdown() noexcept;

    [[nodiscard]]
    static Input_Manager* instance() noexcept;

    [[nodiscard]]
    bool key_pressed(SDL_Scancode key) const noexcept;

    [[nodiscard]]
    bool key_down(SDL_Scancode key) const noexcept;

    [[nodiscard]]
    bool key_released(SDL_Scancode key) const noexcept;
 
    [[nodiscard]]
    bool mouse_pressed(Mouse_Button btn) const noexcept;

    [[nodiscard]]
    bool mouse_down(Mouse_Button btn) const noexcept;

    [[nodiscard]]
    bool mouse_released(Mouse_Button btn) const noexcept;

    [[nodiscard]]
    std::pair<i32, i32> get_mouse() const noexcept;

    void update_prev_state();
    void update_mouse() noexcept;
private:
    const bool* keyboard_state;
    bool* prev_keyboard_state;
    i32 key_length;

    u32 mouse_state;
    u32 prev_mouse_state;
    float mouse_x, mouse_y;

    static inline Input_Manager* instance_ptr {nullptr};

    Input_Manager();
    ~Input_Manager();

    [[nodiscard]]
    uint32_t get_mask(Mouse_Button btn) const noexcept;
};
