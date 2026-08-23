#pragma once

#include "Core/IAction.hpp"

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
            auto damageParam = unit.getParam<DamageType>();
            if (!damageParam)
                return false;

            auto position = world.getPosition(unit);

            auto targets = world.findUnits(
                position,
                MinRange,
                MaxRange,
                [](const sw::core::Unit& target) {
                    return target.getProperty<sw::features::units::properties::MeleeTargetable>();
                });
            if (targets.empty())
                return false;

            auto& target = *targets[0];           // TODO: get random target
            target.applyDamage(*damageParam);

            // Who will log the event? The world?
            return true;
        }
    };
}