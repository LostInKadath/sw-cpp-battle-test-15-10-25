#include "../Framework/TestFramework.hpp"

#include <Core/Properties.hpp>
#include <Core/Unit.hpp>
#include <Core/World.hpp>

#include "../Mocks/MockAction.hpp"
#include "../Mocks/MockHealth.hpp"
#include "../Mocks/MockObserver.hpp"
#include "../Mocks/MockRandom.hpp"

using namespace sw::tests;
using namespace sw::core;

SW_TEST(Unit, SetAndGetProperty)
{
    Unit unit(1, {});
    unit.setProperty<MockHealth, properties::IHealth>(10);

    auto health = unit.getProperty<MockHealth>();
    SW_CHECK(health);
    SW_CHECK(10 == health->value);
}

SW_TEST(Unit, SetAndGetProperty_ByInterface)
{
    Unit unit(1, {});
    unit.setProperty<MockHealth, properties::IHealth>(10);

    auto asInterface = unit.getProperty<properties::IHealth>();
    SW_CHECK(asInterface);
    SW_CHECK(10 == asInterface->currentValue());
}

SW_TEST(Unit, PropertyNotSet_GetPropertyReturnsNull)
{
    Unit unit(1, {});
    SW_CHECK(!unit.getProperty<MockHealth>());
}

SW_TEST(Unit, RemoveProperty)
{
    Unit unit(1, {});
    unit.setProperty<MockHealth, properties::IHealth>(10);
    unit.removeProperty<MockHealth>();

    SW_CHECK(!unit.getProperty<MockHealth>());
}

SW_TEST(Unit, GetId)
{
    Unit unit(42, {});
    SW_CHECK(unit.getId() == 42);
}

SW_TEST(Unit, NoActions_TryActReturnsFalse)
{
    std::vector<std::unique_ptr<IAction>> actions;
    actions.push_back(std::make_unique<MockAction>(false));
    actions.push_back(std::make_unique<MockAction>(false));
    Unit unit(1, std::move(actions));

    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    SW_CHECK(!unit.tryAct(world));
}

SW_TEST(Unit, TryActStopsAtFirstSucceedingAction)
{
    auto first = std::make_unique<MockAction>(false);
    auto second = std::make_unique<MockAction>(true);
    auto third = std::make_unique<MockAction>(true);
    auto* firstPtr = first.get();
    auto* secondPtr = second.get();
    auto* thirdPtr = third.get();

    std::vector<std::unique_ptr<IAction>> actions;
    actions.push_back(std::move(first));
    actions.push_back(std::move(second));
    actions.push_back(std::move(third));
    Unit unit(1, std::move(actions));

    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    SW_CHECK(unit.tryAct(world));
    SW_CHECK(firstPtr->callCount == 1);
    SW_CHECK(secondPtr->callCount == 1);
    SW_CHECK(thirdPtr->callCount == 0);
}
