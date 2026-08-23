#pragma once

#include <functional>
#include <type_traits>
#include <unordered_map>

#include "Core/Point.hpp"
#include "Core/Unit.hpp"

namespace sw::core
{
    struct World
    {
        using UnitId = uint32_t;

        World(uint32_t width, uint32_t height)
            : _width{ width }, _height{ height }
        {
        }

        Point getPosition(const Unit& unit) const
        {
            return _positions.at(unit.getId());
        }

        std::vector<Unit*> findUnits(
            const Point& position,
            size_t minRange,
            size_t maxRange,
            std::function<bool(const Unit&)> filter) const
        {
            std::vector<Unit*> targets;

            // TODO: could loop inside a square? Might be faster than checking all units.
            for (const auto& [id, unit] : _units)
            {
                const auto& unitPosition = _positions.at(id);
                const auto distance = getDistance(unitPosition, position);

                if (minRange <= distance && distance <= maxRange && filter(*unit))
                    targets.push_back(unit.get());
            }

            return targets;
        }

    private:
        // Chebyshev distance -- supports diagonal distance.
        static auto getDistance(const Point& a, const Point& b) -> Point::CoordType
        {
            const auto dx = a.x > b.x ? a.x - b.x : b.x - a.x;
            const auto dy = a.y > b.y ? a.y - b.y : b.y - a.y;
            return std::max(dx, dy);
        }

    private:
        const uint32_t _width{ 0 };
        const uint32_t _height{ 0 };

        std::unordered_map<UnitId, std::unique_ptr<Unit>> _units;
        std::unordered_map<UnitId, Point> _positions;
    };
}
