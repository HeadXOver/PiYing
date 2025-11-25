#pragma once

namespace cus {

    template <typename T>
    inline T max(T a, T b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    inline T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    inline T abs(T a)
    {
        return a < 0 ? -a : a;
    }

    template <typename T>
    inline T clamp(T a, T min, T max)
    {
        return a < min ? min : (a > max ? max : a);
    }

}