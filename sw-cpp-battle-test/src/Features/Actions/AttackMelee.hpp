#pragma once

#include "Core/IAction.hpp"
#include "Core/Unit.hpp"
#include "Core/World.hpp"

#include "Features/Components/UnitProperties.hpp"

namespace sw::features::actions
{
    template <class DamageType>
    struct AttackMelee : public sw::core::IAction
    {
        constexpr static size_t MinRange = 1;
        constexpr static size_t MaxRange = 1;

        bool tryAct(sw::core::Unit& unit, sw::core::World& world) override
        {
            const auto damageParam = unit.getProperty<DamageType>();
            if (!damageParam)
                return false;

            const auto position = world.getPosition(unit);

            const auto targets = world.findUnits(position, MinRange, MaxRange, IsTargetSuitable);
            if (targets.empty())
                return false;

            auto& target = world.pickRandomTarget(targets);

            world.applyDamage(unit, target, damageParam->value);
            return true;
        }

        static bool IsTargetSuitable(const sw::core::Unit& target)
        {
            // Only targets with Health can be attacked.
            const auto health = target.getProperty<sw::features::units::parameters::Health>();
            if (!health || health->value <= 0)
                return false;

            // Only targets that are not immune to Melee can be attacked.
            if (const auto immunities = target.getProperty<sw::features::units::Immunities>())
            {
                if (immunities->types.count(sw::features::units::AttackType::Melee))
                    return false;
            }

            return true;
        }
    };
}
