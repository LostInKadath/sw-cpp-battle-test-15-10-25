#pragma once
#include <cstdint>

namespace sw::core
{
    struct Point
    {
        using CoordType = uint32_t;

        CoordType x{ 0 };
        CoordType y{ 0 };

        bool operator==(const Point& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Point& other) const
        {
            return !operator==(other);
        }
    };
}
