#pragma once

namespace PiYingCus {

    template <typename T>
    inline T max(T a, T b) noexcept
    {
        return (a > b) ? a : b;
    }

    template <typename T>
    inline T min(T a, T b) noexcept
    {
        return (a < b) ? a : b;
    }

    template <typename T1, typename T2>
    inline T1 max(T1 a, T2 b) noexcept
    {
        return (a > b) ? a : b;
    }

    template <typename T1, typename T2>
    inline T1 min(T1 a, T2 b) noexcept
    {
        return (a < b) ? a : b;
    }

    template <typename T>
    inline T abs(T a) noexcept
    {
        return (a < 0) ? -a : a;
    }

    template <typename T>
    inline T clamp(T a, T min, T max) noexcept
    {
        return (a < min) ? min : ((a > max) ? max : a);
    }

}