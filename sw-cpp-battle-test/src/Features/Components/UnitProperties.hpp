#pragma once
#include <algorithm>
#include <unordered_set>

namespace sw::features::units
{
    namespace parameters
    {
        struct Health : public sw::core::properties::IHealth
        {
            int value{ 0 };
            const int maxValue{ 0 };
            
            explicit Health(int value = 0) : value(value), maxValue(value) {}

            bool isDead() const override { return value <= 0; }
            int currentValue() const override { return value; }
            void changeValue(int delta) override {  value = std::clamp(value + delta, 0, maxValue); }
        };
        
        struct Strength{ int value{ 0 }; };
        struct Agility{ int value{ 0 }; };
    }

    enum class AttackType
    {
        Melee,
        Ranged,
    };

    // Set of attack types a unit is immune to.
    // Allows to avoid adding a new property for every new attack type.
    struct Immunities
    {
        std::unordered_set<AttackType> types;
    };
}
