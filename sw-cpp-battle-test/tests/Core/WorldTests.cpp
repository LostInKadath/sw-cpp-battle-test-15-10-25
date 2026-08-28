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

namespace
{
    std::unique_ptr<Unit> makeUnit(uint32_t id, std::vector<std::unique_ptr<IAction>> actions = {})
    {
        return std::make_unique<Unit>(id, std::move(actions));
    }
}

// --- spawnUnit -----------------------------------------------------------

SW_TEST(World, SpawnUnit_ReportsEvent)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(makeUnit(1), "Swordsman", { 2, 3 });

    SW_CHECK(observer.contains("UnitSpawned id=1 type=Swordsman x=2 y=3"));
}

SW_TEST(World, SpawnUnit_DuplicateId_Throws)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });

    SW_CHECK_THROWS(world.spawnUnit(makeUnit(1), "Test", { 1, 1 }));
}

SW_TEST(World, SpawnUnit_PositionOutOfBounds_Throws)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    SW_CHECK_THROWS(world.spawnUnit(makeUnit(1), "Test", { 50, 50 }));
}

SW_TEST(World, SpawnUnit_CellOccupied_Throws)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });

    SW_CHECK_THROWS(world.spawnUnit(makeUnit(2), "Test", { 0, 0 }));
}

// --- findUnits / isCellBlocked -------------------------------------------

SW_TEST(World, FindUnits_UsesChebyshevRangeInclusive)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(makeUnit(1), "Test", { 5, 5 });
    world.spawnUnit(makeUnit(2), "Test", { 6, 6 }); // diagonal, distance 1
    world.spawnUnit(makeUnit(3), "Test", { 7, 5 }); // distance 2

    auto found = world.findUnits({ 5, 5 }, 1, 1, [](const Unit&) { return true; });

    SW_CHECK(found.size() == 1);
    SW_CHECK(found[0]->getId() == 2);
}

SW_TEST(World, FindUnits_AppliesFilter)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(makeUnit(1), "Test", { 5, 5 });
    world.spawnUnit(makeUnit(2), "Test", { 6, 6 });
    world.spawnUnit(makeUnit(3), "Test", { 5, 6 });
    world.spawnUnit(makeUnit(4), "Test", { 6, 5 });

    auto found = world.findUnits({ 5, 5 }, 0, 5, [](const Unit& unit) { return unit.getId() == 2; });

    SW_CHECK(found.size() == 1);
    SW_CHECK(found[0]->getId() == 2);
}

SW_TEST(World, IsCellBlocked_EmptyCell_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    SW_CHECK(!world.isCellBlocked({ 3, 3 }));
}

SW_TEST(World, IsCellBlocked_OccupyingUnit_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(makeUnit(1), "Test", { 3, 3 });

    SW_CHECK(world.isCellBlocked({ 3, 3 }));
}

SW_TEST(World, IsCellBlocked_NotOccupiesUnit_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    auto unit = makeUnit(1);
    unit->setProperty<properties::NotOccupies>();
    world.spawnUnit(std::move(unit), "Test", { 3, 3 });

    SW_CHECK(!world.isCellBlocked({ 3, 3 }));
}

// --- pickRandomTarget ------------------------------------------------------

SW_TEST(World, PickRandomTarget_ReturnsElementAtScriptedIndex)
{
    MockObserver observer;
    MockRandom random(2);   // returns 2 every next() call
    World world(10, 10, observer, random);

    Unit a(0, {}), b(1, {}), c(2, {});
    std::vector<Unit*> targets{ &a, &b, &c };

    auto& picked = world.pickRandomTarget(targets);

    SW_CHECK(picked.getId() == 2);
    SW_CHECK(random.lastExclusiveMax() == 3);   
}

SW_TEST(World, PickRandomTarget_EmptyList_Throws)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    std::vector<Unit*> empty;
    SW_CHECK_THROWS(world.pickRandomTarget(empty));
}

// --- moveUnit / startMarch / completeMarch --------------------------------

SW_TEST(World, MoveUnit_UpdatesPositionAndReports)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });

    world.moveUnit(*world.getUnit(1), { 1, 1 });

    auto position = world.getPosition(*world.getUnit(1));
    SW_CHECK(1 == position.x);
    SW_CHECK(1 == position.y);
    SW_CHECK(observer.contains("UnitMoved id=1"));
}

SW_TEST(World, MoveUnit_OutOfBounds_Throws)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });

    SW_CHECK_THROWS(world.moveUnit(*world.getUnit(1), { 100, 100 }));
}

SW_TEST(World, StartMarch_SetsPropertyAndReportsEvent)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });

    world.startMarch(1, { 5, 5 });

    auto march = world.getUnit(1)->getProperty<properties::March>();
    SW_CHECK(march);
    SW_CHECK(5 == march->target.x);
    SW_CHECK(5 == march->target.y);
    SW_CHECK(observer.contains("MarchStarted id=1"));
}

SW_TEST(World, StartMarch_TargetOutOfBounds_Throws)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });

    SW_CHECK_THROWS(world.startMarch(1, { 50, 50 }));
}

SW_TEST(World, CompleteMarch_RemovesPropertyAndReportsEvent)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });
    world.startMarch(1, { 5, 5 });

    world.completeMarch(*world.getUnit(1));

    SW_CHECK(!world.getUnit(1)->getProperty<properties::March>());
    SW_CHECK(observer.contains("MarchEnded id=1"));
}

// --- applyDamage / healUnit ------------------------------------------------

SW_TEST(World, ApplyDamage_ReducesHealthAndReports)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Attacker", { 0, 0 });

    auto target = makeUnit(2);
    target->setProperty<MockHealth, properties::IHealth>(10);
    world.spawnUnit(std::move(target), "Target", { 1, 1 });

    world.applyDamage(*world.getUnit(1), *world.getUnit(2), 4);

    auto health = world.getUnit(2)->getProperty<properties::IHealth>();
    SW_CHECK(6 == health->currentValue());
    SW_CHECK(observer.contains("UnitAttacked attacker=1 target=2 damage=4 targetHealth=6"));
}

SW_TEST(World, ApplyDamage_TargetHasNoHealth_DoesNothing)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Attacker", { 0, 0 });
    world.spawnUnit(makeUnit(2), "Target", { 1, 1 });

    world.applyDamage(*world.getUnit(1), *world.getUnit(2), 4); // doesn't throw

    SW_CHECK(observer.contains("UnitAttacked attacker=1 target=2 damage=4 targetHealth=0"));
}

SW_TEST(World, HealUnit_IncreasesHealth)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Healer", { 0, 0 });

    auto target = makeUnit(2);
    target->setProperty<MockHealth, properties::IHealth>(3);
    world.spawnUnit(std::move(target), "Target", { 1, 1 });

    world.healUnit(*world.getUnit(1), *world.getUnit(2), 5);

    // Here we check only healUnit() mechanics -- MockHealth increases value beyond maxValue, but we don't care =)
    SW_CHECK(8 == world.getUnit(2)->getProperty<properties::IHealth>()->currentValue());
}

// --- step() ------------------------------------------------------------

SW_TEST(World, Step_ActsUnitsInCreationOrder)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    std::vector<int> callLog;

    std::vector<std::unique_ptr<IAction>> a1;
    a1.push_back(std::make_unique<MockAction>(true, &callLog, 15));
    world.spawnUnit(makeUnit(15, std::move(a1)), "Test", { 0, 0 });

    std::vector<std::unique_ptr<IAction>> a2;
    a2.push_back(std::make_unique<MockAction>(true, &callLog, 28));
    world.spawnUnit(makeUnit(28, std::move(a2)), "Test", { 1, 1 });

    std::vector<std::unique_ptr<IAction>> a3;
    a3.push_back(std::make_unique<MockAction>(true, &callLog, 3));
    world.spawnUnit(makeUnit(3, std::move(a3)), "Test", { 2, 2 });

    world.step(1);

    SW_CHECK((callLog == std::vector<int>{ 15, 28, 3 }));   // IDs are in creation order, not sorted
}

SW_TEST(World, Step_SingleUnitRemains_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });

    SW_CHECK(!world.step(1));
}

SW_TEST(World, Step_NobodyActed_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    std::vector<std::unique_ptr<IAction>> a1;
    a1.push_back(std::make_unique<MockAction>(false));
    world.spawnUnit(makeUnit(1, std::move(a1)), "Test", { 0, 0 });

    std::vector<std::unique_ptr<IAction>> a2;
    a2.push_back(std::make_unique<MockAction>(false));
    world.spawnUnit(makeUnit(2, std::move(a2)), "Test", { 1, 1 });

    SW_CHECK(!world.step(1));
}

SW_TEST(World, Step_SomeoneActedAndMultipleUnitsRemain_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    std::vector<std::unique_ptr<IAction>> unit;
    unit.push_back(std::make_unique<MockAction>(true));
    world.spawnUnit(makeUnit(1, std::move(unit)), "Test", { 0, 0 });
    world.spawnUnit(makeUnit(2), "Test", { 1, 1 });

    SW_CHECK(world.step(1));
}

SW_TEST(World, Step_DeadUnitActsBeforeRemoving)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    std::vector<int> callLog;

    std::vector<std::unique_ptr<IAction>> a1;
    a1.push_back(std::make_unique<MockAction>(true, &callLog, 1));
    auto unit1 = makeUnit(1, std::move(a1));
    unit1->setProperty<MockHealth, properties::IHealth>(0); // already dead
    world.spawnUnit(std::move(unit1), "Test", { 0, 0 });

    std::vector<std::unique_ptr<IAction>> a2;
    a2.push_back(std::make_unique<MockAction>(true, &callLog, 2));
    world.spawnUnit(makeUnit(2, std::move(a2)), "Test", { 1, 1 });

    world.step(1);

    SW_CHECK((callLog == std::vector<int>{ 1, 2 })); // dead unit 1 still acted this step
    SW_CHECK(observer.contains("UnitDied id=1"));
    SW_CHECK(!observer.contains("UnitDied id=2"));
}

SW_TEST(World, Step_RemovesDeadUnit)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    auto unit1 = makeUnit(1);
    unit1->setProperty<MockHealth, properties::IHealth>(0);     // unit is dead, 0  HP
    world.spawnUnit(std::move(unit1), "Test", { 0, 0 });
    world.spawnUnit(makeUnit(2), "Test", { 1, 1 });             // unit is alive, despite no Health stat

    world.step(1);

    auto everyone = world.findUnits({ 0, 0 }, 0, 100, [](const Unit&) { return true; });
    SW_CHECK(everyone.size() == 1);
    SW_CHECK(everyone[0]->getId() == 2);
}

SW_TEST(World, Step_UnitsWithoutHealthRemainOnMap)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);

    world.spawnUnit(makeUnit(1), "Test", { 0, 0 });
    world.spawnUnit(makeUnit(2), "Test", { 1, 1 });

    world.step(1);

    auto everyone = world.findUnits({ 0, 0 }, 0, 100, [](const Unit&) { return true; });
    SW_CHECK(everyone.size() == 2);
    SW_CHECK(!observer.contains("UnitDied"));
}
