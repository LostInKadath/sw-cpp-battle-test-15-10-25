#include "../../Framework/TestFramework.hpp"

#include <Features/Units/Swordsman.hpp>

using namespace sw::tests;
using namespace sw::core;
using namespace sw::features::units;

SW_TEST(Swordsman, HasHealthAsTypeAndInterface)
{
    auto unit = Swordsman::create(1, 10, 5);

    auto health = unit->getProperty<parameters::Health>();
    SW_CHECK(health);
    SW_CHECK(10 == health->currentValue());

    // Health must also be reachable via IHealth
    auto asInterface = unit->getProperty<properties::IHealth>();
    SW_CHECK(asInterface);
    SW_CHECK(10 == asInterface->currentValue());
}

SW_TEST(Swordsman, HasStrength)
{
    auto unit = Swordsman::create(1, 10, 7);

    auto strength = unit->getProperty<parameters::Strength>();
    SW_CHECK(strength);
    SW_CHECK(7 == strength->value);
}

SW_TEST(Swordsman, HasNoAgilityOrRangedAttack)
{
    auto unit = Swordsman::create(1, 10, 7);
    SW_CHECK(unit->getProperty<parameters::Agility>() == nullptr);
}
