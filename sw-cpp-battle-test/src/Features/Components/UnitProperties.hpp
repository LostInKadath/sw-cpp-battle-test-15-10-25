#pragma once
#include <unordered_set>

namespace sw::features::units
{
    namespace parameters
    {
        struct Health : public sw::core::properties::IHealth
        {
            int value{ 0 };
            
            explicit Health(uint32_t value = 0) : value(value) {}

            bool isDead() const override { return 0 == value; }
            uint32_t currentValue() const override { return value; }
            void changeValue(uint32_t delta) override {  value += delta; }
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
