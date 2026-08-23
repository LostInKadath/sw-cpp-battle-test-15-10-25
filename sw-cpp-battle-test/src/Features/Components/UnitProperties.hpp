#pragma once
#include <unordered_set>

namespace sw::features::units
{
    namespace parameters
    {
        struct Health{ int value{ 0 }; };
        struct Strength{ int value{ 0 }; };
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
