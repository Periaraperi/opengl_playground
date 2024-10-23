#pragma once

#ifdef PERIA_DEBUG
#include <iostream>

template<typename T>
struct Add_Space {
    const T& elem;

    explicit Add_Space(const T& t): elem(t) {}

    friend std::ostream& operator<<(std::ostream& os, const Add_Space<T>& e)
    { os << e.elem << ' '; return os; }
};

namespace peria {

template<typename... T>
void log(T&&... args)
{
    (std::cerr << ... << Add_Space(args)) << '\n';
}

}

#else
namespace peria {

template<typename... T>
void log([[maybe_unused]] T&&... args)
{ return; }

}
#endif
