#include "timer.hpp"
#include "simple_logger.hpp"

namespace peria {

Timer::Timer()
{ peria::log("Timer ctor()"); }

Timer::~Timer()
{ peria::log("Timer dtor()"); }


void Timer::initialize() noexcept
{ 
    if (timer_instance == nullptr) {
        timer_instance = new Timer{};
    }
}

void Timer::shutdown() noexcept
{ delete timer_instance; timer_instance = nullptr; }

Timer* Timer::instance() noexcept
{ return timer_instance;}

void Timer::update() noexcept
{
    curr_time = clock.now();
    const auto elapsed_time {std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - prev_time).count()};
    delta = elapsed_time * 0.001f;
    accum += delta;
    prev_time = curr_time;
}

bool Timer::do_fixed_step() noexcept
{
    if (accum >= fixed) {
        accum -= fixed;
        leftover = true;
        return true;
    }
    return false;
}

float Timer::leftover_accum() noexcept
{
    if (leftover) {
        return accum;
    }
    return 0.0f;
}

float Timer::dt() noexcept
{ return delta; }

}
