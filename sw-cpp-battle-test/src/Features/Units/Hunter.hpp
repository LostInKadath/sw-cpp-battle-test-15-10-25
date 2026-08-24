#pragma once

#include <memory>
#include <vector>

#include <Core/Unit.hpp>
#include <Core/World.hpp>
#include <Features/Actions/AttackMelee.hpp>
#include <Features/Actions/AttackRanged.hpp>
#include <Features/Actions/Move.hpp>
#include <Features/Components/UnitProperties.hpp>

namespace sw::features::units
{
    struct Hunter
    {
        constexpr static const char Name[] = "Hunter";

        static std::unique_ptr<sw::core::Unit> create(uint32_t id, uint32_t health, uint32_t agility, uint32_t strength, uint32_t range)
        {
            using namespace sw::core;
            using namespace sw::features;
            
            auto noNeighbours = [](const World& world, const Point& position) {
                auto anyUnit = [](const Unit&) { return true; };
                return world.findUnits(position, 1, 1, anyUnit).empty();
            };

            std::vector<std::unique_ptr<IAction>> actions;
            actions.push_back(std::make_unique<actions::AttackRanged<units::parameters::Agility>>(2, range, std::move(noNeighbours)));
            actions.push_back(std::make_unique<actions::AttackMelee<units::parameters::Strength>>());
            actions.push_back(std::make_unique<actions::Move<>>());

            auto unit = std::make_unique<Unit>(id, std::move(actions));

            unit->setProperty<units::parameters::Health, properties::IHealth>(health);
            unit->setProperty<units::parameters::Strength>(strength);
            unit->setProperty<units::parameters::Agility>(agility);

            return unit;
        }
    };
}