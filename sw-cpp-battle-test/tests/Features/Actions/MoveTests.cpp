#include "../../Framework/TestFramework.hpp"

#include <Core/Properties.hpp>
#include <Core/Unit.hpp>
#include <Core/World.hpp>
#include <Features/Actions/Move.hpp>

#include "../../Mocks/MockObserver.hpp"
#include "../../Mocks/MockRandom.hpp"

using namespace sw::tests;
using namespace sw::core;
using namespace sw::features::actions;

SW_TEST(Move, NoMarch_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Test", { 0, 0 });
    // No March property

    Move<> move;
    SW_CHECK(!move.tryAct(*world.getUnit(1), world));
}

SW_TEST(Move, BlockedOnFirstStep_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Mover", { 0, 0 });
    world.spawnUnit(std::make_unique<Unit>(2, std::vector<std::unique_ptr<IAction>>{}), "Blocker", { 1, 0 });
    world.startMarch(1, { 5, 0 });

    Move<> move;
    SW_CHECK(!move.tryAct(*world.getUnit(1), world));
    SW_CHECK(!observer.contains("UnitMoved id=1"));
}

SW_TEST(Move, MoveOneStep_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Mover", { 0, 0 });
    world.startMarch(1, { 5, 0 });

    Move<> move;
    SW_CHECK(move.tryAct(*world.getUnit(1), world));
    SW_CHECK(1 == world.getPosition(*world.getUnit(1)).x);
    SW_CHECK(observer.contains("UnitMoved id=1"));
}

SW_TEST(Move, TargetIsReached_MarchCompleted_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Mover", { 0, 0 });
    world.startMarch(1, { 1, 0 }); // one step away, default Speed = 1

    Move<> move;
    SW_CHECK(move.tryAct(*world.getUnit(1), world));
    SW_CHECK(!world.getUnit(1)->getProperty<properties::March>());
    SW_CHECK(observer.contains("MarchEnded id=1"));
}

SW_TEST(Move, TargetIsReachedOnHalfMove_MarchCompleted_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Mover", { 0, 0 });
    world.startMarch(1, { 1, 0 }); // one step away, default Speed = 2

    Move<2> move;
    SW_CHECK(move.tryAct(*world.getUnit(1), world));
    SW_CHECK(!world.getUnit(1)->getProperty<properties::March>());
    SW_CHECK(observer.contains("MarchEnded id=1"));
}

SW_TEST(Move, BlockedMidWay_MarchInProgress_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Mover", { 0, 0 });
    world.spawnUnit(std::make_unique<Unit>(2, std::vector<std::unique_ptr<IAction>>{}), "Blocker", { 2, 0 });
    world.startMarch(1, { 5, 0 });

    Move<2> move;
    SW_CHECK(move.tryAct(*world.getUnit(1), world)); // step 1 succeeds, step 2 is blocked -- still progress
    SW_CHECK(1 == world.getPosition(*world.getUnit(1)).x);
    SW_CHECK(world.getUnit(1)->getProperty<properties::March>()); // march not completed
}
