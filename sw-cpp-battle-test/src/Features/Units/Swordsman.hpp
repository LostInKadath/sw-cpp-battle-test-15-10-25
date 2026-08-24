#pragma once

#include <memory>
#include <vector>

#include "Core/Unit.hpp"
#include "Features/Actions/AttackMelee.hpp"
#include <Features/Actions/Move.hpp>
#include "Features/Components/UnitProperties.hpp"

namespace sw::features::units
{
    struct Swordsman
    {
        constexpr static const char Name[] = "Swordsman";

        static std::unique_ptr<sw::core::Unit> create(uint32_t id, uint32_t health, uint32_t strength)
        {
            using namespace sw::core;
            using namespace sw::features;

            std::vector<std::unique_ptr<IAction>> actions;
            actions.push_back(std::make_unique<actions::AttackMelee<units::parameters::Strength>>());
            actions.push_back(std::make_unique<actions::Move<>>());

            auto unit = std::make_unique<Unit>(id, std::move(actions));

            unit->setProperty<units::parameters::Health, properties::IHealth>(health);
            unit->setProperty<units::parameters::Strength>(strength);

            return unit;
        }
    };
}
