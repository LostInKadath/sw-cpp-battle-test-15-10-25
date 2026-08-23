#pragma once

#include <functional>

#include "Core/IAction.hpp"

#include "Features/Components/UnitProperties.hpp"

namespace sw::features::actions
{
    template <class DamageType>
    struct AttackRanged : public sw::core::IAction
    {
        using ConditionFn = std::function<bool(const sw::core::World& world, const sw::core::Point&)>;

        AttackRanged(uint32_t minRange, uint32_t maxRange, ConditionFn condition = nullptr)
            : _minRange{ minRange }, _maxRange{ maxRange }, _condition{ std::move(condition) }
        {}

        bool tryAct(sw::core::Unit& unit, sw::core::World& world) override
        {
            const auto damageParam = unit.getProperty<DamageType>();
            if (!damageParam)
                return false;

            const auto position = world.getPosition(unit);

            if (_condition && !_condition(world, position))
                // Something near our position prevents us from attacking.
                return false;

            const auto targets = world.findUnits(position, _minRange, _maxRange, IsTargetSuitable);
            if (targets.empty())
                return false;

            auto& target = *targets[0];           // TODO: get random target

            if (auto targetHealth = target.getProperty<units::parameters::Health>())
                targetHealth->value = std::max(0, targetHealth->value - damageParam->value);

            // TODO: log the attack
            return true;
        }

    private:
        static bool IsTargetSuitable(const sw::core::Unit& target)
        {
            // Only targets with Health can be attacked.
            const auto health = target.getProperty<sw::features::units::parameters::Health>();
            if (!health || health->value <= 0)
                return false;

            // Only targets that are not immune to Range attack can be attacked.
            if (const auto immunities = target.getProperty<sw::features::units::Immunities>())
            {
                if (immunities->types.count(sw::features::units::AttackType::Ranged))
                    return false;
            }
            
            return true;
        }

    private:
        const uint32_t _minRange;
        const uint32_t _maxRange;
        const ConditionFn _condition;
    };
}
