#pragma once

#include <functional>
#include <type_traits>
#include <unordered_map>

#include <Core/ISimulationObserver.hpp>
#include <Core/Point.hpp>
#include <Core/Properties.hpp>
#include <Core/Unit.hpp>

namespace sw::core
{
    struct World
    {
        using UnitId = uint32_t;

        World(uint32_t width, uint32_t height, const ISimulationObserver& observer)
            : _width{ width }, _height{ height }, _observer{ observer }
        {
            _observer.onMapCreated(_currentTick, _width, _height);
        }

        void spawnUnit(std::unique_ptr<Unit>&& unit, std::string_view type, const Point& position)
        {
            if (!unit)
                throw std::runtime_error("No unit provided!");

            const auto id = unit->getId();
            if (_units.contains(id))
                throw std::runtime_error("Unit with id " + std::to_string(id) + " already exists!");

            if (isCellBlocked(position))
                throw std::runtime_error("Position " + std::to_string(position.x) + ", " + std::to_string(position.y) + " is already occupied!");

            _units.emplace(id, std::move(unit));
            _positions.emplace(id, position);

            _observer.onUnitSpawned(_currentTick, id, type, position);
        }

        std::unique_ptr<Unit>& getUnit(UnitId id)
        {
            return _units.at(id);
        }

        Point getPosition(const Unit& unit) const
        {
            return _positions.at(unit.getId());
        }

        bool isCellBlocked(const Point& position) const
        {
            constexpr const auto MinRange = 0;
            constexpr const auto MaxRange = 0;

            auto conflictingUnits = findUnits(position, MinRange, MaxRange, [](const Unit& unit) {
                return !unit.getProperty<properties::NotOccupies>();
            });
            return !conflictingUnits.empty();
        }

        std::vector<Unit*> findUnits(
            const Point& position,
            size_t minRange,
            size_t maxRange,
            const std::function<bool(const Unit&)>& filter) const
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

        void moveUnit(const Unit& unit, const Point& position)
        {
            const auto id = unit.getId();
            _positions.at(id) = position;
            _observer.onUnitMoved(_currentTick, id, position);
        }

        void completeMarch(Unit& unit)
        {
            unit.removeProperty<properties::March>();
            _observer.onMarchEnded(_currentTick, unit.getId(), _positions.at(unit.getId()));
        }

    public: // Wrappers to inject _currentTick
        void startMarch(uint32_t unitId, const Point& to)
        {
            const auto from =_positions.at(unitId);
            _observer.onMarchStarted(_currentTick, unitId, from, to);
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
        const ISimulationObserver& _observer;

        std::unordered_map<UnitId, std::unique_ptr<Unit>> _units;
        std::unordered_map<UnitId, Point> _positions;

        uint32_t _currentTick{ 0 };
    };
}
