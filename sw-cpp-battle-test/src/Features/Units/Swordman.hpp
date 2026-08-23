#pragma once

#include <memory>
#include <vector>

#include "Core/Unit.hpp"
#include "Features/Actions/AttackMelee.hpp"
#include "Features/Components/UnitProperties.hpp"

namespace sw::features::units
{
    struct Swordman
    {
        constexpr static const char Name[] = "Swordsman";

        static std::unique_ptr<sw::core::Unit> create(uint32_t id, uint32_t health, uint32_t strength)
        {
            using namespace sw::core;

            std::vector<std::unique_ptr<IAction>> actions;
            actions.push_back(std::make_unique<sw::features::actions::AttackMelee<sw::features::units::parameters::Strength>>());

            auto unit = std::make_unique<Unit>(id, std::move(actions));

            unit->setProperty<sw::features::units::parameters::Health>(health);
            unit->setProperty<sw::features::units::parameters::Strength>(strength);

            // TODO: bad, as needs every NewAttackTypeVulnerable to be added for every existing unit type.
            // Think about default property values for all units. Maybe property should be NotMeleeTargetable?
            unit->setProperty<sw::features::units::properties::MeleeTargetable>();

            return unit;
        }
    };
}
