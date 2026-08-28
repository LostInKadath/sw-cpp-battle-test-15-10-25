#include "../../Framework/TestFramework.hpp"

#include <Core/Unit.hpp>
#include <Core/World.hpp>
#include <Features/Units/Hunter.hpp>
#include <Features/Components/UnitProperties.hpp>

#include "../../Mocks/MockObserver.hpp"
#include "../../Mocks/MockRandom.hpp"

using namespace sw::core;
using namespace sw::tests;
using namespace sw::features::units::parameters;

SW_TEST(Health, IsDeadWhenValueIsZeroOrLess)
{
    Health health(5);
    SW_CHECK(!health.isDead());

    health.changeValue(-5);
    SW_CHECK(health.isDead());

    health.changeValue(-100);
    SW_CHECK(health.isDead());
}

SW_TEST(Health, ChangeValueClampsAtSpawnMaxHp)
{
    Health health(10);
    health.changeValue(-3);
    SW_CHECK(7 == health.currentValue());

    health.changeValue(+100);
    SW_CHECK(10 == health.currentValue());  // healing must not exceed the HP the unit spawned with
}

SW_TEST(Health, ChangeValueClampsAtZero)
{
    Health health(10);
    health.changeValue(-1000);
    SW_CHECK(health.currentValue() == 0);
}

SW_TEST(Health, SynchronizedChangesForTypeAndInterface)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Attacker", {0, 0});

    auto target = std::make_unique<Unit>(2, std::vector<std::unique_ptr<IAction>>{});
    auto targetPtr = target.get();

    target->setProperty<Health, properties::IHealth>(10);
    world.spawnUnit(std::move(target), "Target", {1, 0});

    world.applyDamage(*world.getUnit(1), *world.getUnit(2), 5);

    auto health = targetPtr->getProperty<Health>();
    SW_CHECK(health);
    SW_CHECK(5 == health->currentValue());

    auto asInterface = targetPtr->getProperty<properties::IHealth>();
    SW_CHECK(asInterface);
    SW_CHECK(5 == asInterface->currentValue());
}
