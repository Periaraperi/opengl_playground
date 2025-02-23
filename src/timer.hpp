#pragma once

#include <chrono>
namespace peria {

class Timer {
public:
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    Timer(Timer&& rhs) = delete;
    Timer& operator=(Timer&& rhs) = delete;
    
    static void initialize() noexcept;
    static void shutdown() noexcept;

    [[nodiscard]]
    static Timer* instance() noexcept;

    constexpr static float fixed {1.0f / 60.0f}; // in seconds

    void update() noexcept;

    [[nodiscard]]
    bool do_fixed_step() noexcept;

    [[nodiscard]]
    float dt() noexcept;

    [[nodiscard]]
    float leftover_accum() noexcept;
private:
    Timer();
    ~Timer();
    static inline Timer* timer_instance {nullptr};

    float delta    {0.0f};         // in seconds
    float accum    {0.0f};         // in seconds
    bool  leftover {false};

    std::chrono::system_clock clock {};
    std::chrono::time_point<std::chrono::system_clock> prev_time {clock.now()};
    std::chrono::time_point<std::chrono::system_clock> curr_time {clock.now()};

};


}
