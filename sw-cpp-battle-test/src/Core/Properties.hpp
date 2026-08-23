#pragma once

#include <Core/Point.hpp>

// Unit properties and parameters, that have a direct impact on the core mechanics of the game.
namespace sw::core::properties
{
    // Unit doesn't occupy position on the map.
    struct NotOccupies {};

    // March command for a unit.
    struct March
    {
        Point target{};
    };
}
