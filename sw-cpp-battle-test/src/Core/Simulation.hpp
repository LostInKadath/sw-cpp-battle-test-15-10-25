#pragma once

#include <optional>
#include <stdexcept>

#include <Core/World.hpp>
#include <Features/Units/Hunter.hpp>
#include <Features/Units/Swordsman.hpp>

namespace sw::core
{
    struct Simulation
    {
        void createMap(uint32_t width, uint32_t height)
        {
            _world.emplace(width, height);
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
        }

    private:
        std::optional<World> _world;
    };
}