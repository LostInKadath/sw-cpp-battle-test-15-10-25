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
            auto unit = features::units::Swordsman::create(unitId, health, strength);
		    _world->spawnUnit(std::move(unit), {x, y});
        }

        void spawnHunter(uint32_t unitId, uint32_t x, uint32_t y, uint32_t health, uint32_t agility, uint32_t strength, uint32_t range)
        {
            auto unit = features::units::Hunter::create(unitId, health, agility, strength, range);
		    _world->spawnUnit(std::move(unit), {x, y});
        }

        void marchUnit(uint32_t unitId, uint32_t targetX, uint32_t targetY)
        {
            auto& unit = _world->getUnit(unitId);
            unit->setProperty<core::properties::March>(Point{targetX, targetY});

            // Notifying the _observer
            _world->onMarchStarted(unitId, {targetX, targetY});
        }

    private:
        const ISimulationObserver& _observer;
        std::optional<World> _world;
    };
}