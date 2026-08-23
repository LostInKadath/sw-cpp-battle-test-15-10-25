#pragma once

#include <optional>
#include <stdexcept>

#include <Core/World.hpp>
#include <Features/Units/Hunter.hpp>
#include <Features/Units/Swordsman.hpp>

#include <Core/ISimulationObserver.hpp>

namespace sw::core
{
    struct Simulation
    {
        Simulation(ISimulationObserver& observer)
            : _observer{ observer }
        {}

        void createMap(uint32_t width, uint32_t height)
        {
            _world.emplace(width, height, _observer);
        }

        void spawnSwordsman(uint32_t unitId, uint32_t x, uint32_t y, uint32_t health, uint32_t strength)
        {
            spawnUnit<features::units::Swordsman>(unitId, x, y, health, strength);
        }

        void spawnHunter(uint32_t unitId, uint32_t x, uint32_t y, uint32_t health, uint32_t agility, uint32_t strength, uint32_t range)
        {
            spawnUnit<features::units::Hunter>(unitId, x, y, health, agility, strength, range);
        }

        void marchUnit(uint32_t unitId, uint32_t targetX, uint32_t targetY)
        {
            auto& unit = _world->getUnit(unitId);
            unit->setProperty<core::properties::March>(Point{targetX, targetY});

            // Notifying the _observer -- a bit ugly...
            _world->startMarch(unitId, {targetX, targetY});
        }

    private:
        template <typename UnitType, typename... Args>
        void spawnUnit(uint32_t unitId, uint32_t x, uint32_t y, Args&&... args)
        {
            auto unit = UnitType::create(unitId, std::forward<Args>(args)...);
		    _world->spawnUnit(std::move(unit), UnitType::Name, {x, y});
        }

    private:
        const ISimulationObserver& _observer;
        std::optional<World> _world;
    };
}