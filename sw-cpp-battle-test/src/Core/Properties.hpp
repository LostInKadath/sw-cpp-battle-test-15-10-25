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

    struct IHealth
    {
        virtual ~IHealth() = default;
        virtual bool isDead() const = 0;
        virtual int currentValue() const = 0;
        virtual void changeValue(int delta) = 0;
    };
}
