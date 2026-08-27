#pragma once

#include <optional>
#include <stdexcept>

#include <Core/Randomizer.hpp>
#include <Core/World.hpp>
#include <Features/Units/Hunter.hpp>
#include <Features/Units/Swordsman.hpp>

#include <Core/ISimulationObserver.hpp>

namespace sw::core
{
    struct Simulation
    {
        Simulation(ISimulationObserver&);

        void createMap(uint32_t width, uint32_t height);

        void spawnSwordsman(uint32_t unitId, uint32_t x, uint32_t y, uint32_t health, uint32_t strength);
        void spawnHunter(uint32_t unitId, uint32_t x, uint32_t y, uint32_t health, uint32_t agility, uint32_t strength, uint32_t range);

        void marchUnit(uint32_t unitId, uint32_t targetX, uint32_t targetY);

        void run();

    private:
        template <typename UnitType, typename... Args>
        void spawnUnit(uint32_t unitId, uint32_t x, uint32_t y, Args&&... args)
        {
            auto unit = UnitType::create(unitId, std::forward<Args>(args)...);
		    _world->spawnUnit(std::move(unit), UnitType::Name, {x, y});
        }

    private:
        const ISimulationObserver& _observer;
        Randomizer _random;
        std::optional<World> _world;
    };
}
