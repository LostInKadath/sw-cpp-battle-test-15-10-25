#pragma once
#include <cstdint>

namespace sw::core
{
    struct Point
    {
        using CoordType = uint32_t;

        CoordType x{ 0 };
        CoordType y{ 0 };
    };
}
