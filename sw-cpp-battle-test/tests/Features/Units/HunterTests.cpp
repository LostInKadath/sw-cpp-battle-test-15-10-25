#include "../../Framework/TestFramework.hpp"

#include <Core/Unit.hpp>
#include <Core/World.hpp>
#include <Features/Units/Hunter.hpp>

#include "../../Mocks/MockObserver.hpp"
#include "../../Mocks/MockRandom.hpp"

using namespace sw::tests;
using namespace sw::core;
using namespace sw::features::units;

SW_TEST(Hunter, HasHealthAsTypeAndInterface)
{
    auto unit = Hunter::create(1, 10, 3, 2, 4);

    auto health = unit->getProperty<parameters::Health>();
    SW_CHECK(health);
    SW_CHECK(10 == health->currentValue());

    // Health must also be reachable via IHealth
    auto asInterface = unit->getProperty<properties::IHealth>();
    SW_CHECK(asInterface);
    SW_CHECK(10 == asInterface->currentValue());
}

SW_TEST(Hunter, HasAgilityAndStrength)
{
    auto unit = Hunter::create(1, 10, 3, 2, 4);

    auto agility = unit->getProperty<parameters::Agility>();
    SW_CHECK(agility);
    SW_CHECK(3 == agility->value);

    auto strength = unit->getProperty<parameters::Strength>();
    SW_CHECK(strength);
    SW_CHECK(2 == strength->value);
}

SW_TEST(Hunter, HasNeighbours_SkipRangedAttack)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    auto hunter = Hunter::create(1, 10, 3, 2, 4);
    world.spawnUnit(std::move(hunter), "Hunter", { 0, 0 });

    auto target = Hunter::create(2, 10, 0, 0, 0);
    world.spawnUnit(std::move(target), "Target", { 3, 0 }); // within ranged range

    // Neighbour; has 0 HP -- not a valid target
    auto neighbour = Hunter::create(3, 0, 0, 0, 0);
    world.spawnUnit(std::move(neighbour), "Neighbour", { 0, 1 }); // next to the hunter

    // Ranged attack skipped, as hunter has neighbours.
    // Melee attack skipped, as target has 0 HP.
    // March skipped -- no march target.
    SW_CHECK(!world.getUnit(1)->tryAct(world));
}
