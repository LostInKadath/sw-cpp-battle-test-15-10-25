#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include <Core/ISimulationObserver.hpp>
#include <Core/Point.hpp>
#include <Core/Properties.hpp>
#include <Core/Unit.hpp>

namespace sw::core
{
    struct World
    {
        using UnitId = uint32_t;

    public:
        World(uint32_t width, uint32_t height, const ISimulationObserver&);

        void spawnUnit(std::unique_ptr<Unit>&& unit, std::string_view type, const Point& position);
        std::unique_ptr<Unit>& getUnit(UnitId id);

        Point getPosition(const Unit& unit) const;
        bool isCellBlocked(const Point& position) const;

        std::vector<Unit*> findUnits(
            const Point& position,
            size_t minRange,
            size_t maxRange,
            const std::function<bool(const Unit&)>& filter) const;

        void moveUnit(const Unit& unit, const Point& position);

        void startMarch(uint32_t unitId, const Point& to);
        void completeMarch(Unit& unit);

        void applyDamage(Unit& attacker, Unit& target, uint32_t amount);
        void healUnit(Unit& healer, Unit& target, uint32_t amount);

    public:
        bool step(uint32_t tick);

    private:
        // Chebyshev distance -- supports diagonal distance.
        static auto getDistance(const Point& a, const Point& b) -> Point::CoordType
        {
            const auto dx = a.x > b.x ? a.x - b.x : b.x - a.x;
            const auto dy = a.y > b.y ? a.y - b.y : b.y - a.y;
            return std::max(dx, dy);
        }

        bool isPositionOnMap(const Point& p) const;

    private:
        const uint32_t _width{ 0 };
        const uint32_t _height{ 0 };
        const ISimulationObserver& _observer;

        std::unordered_map<UnitId, std::unique_ptr<Unit>> _units;
        std::unordered_map<UnitId, Point> _positions;
        std::vector<UnitId> _turnOrder;

        uint32_t _currentTick{ 0 };
    };
}
