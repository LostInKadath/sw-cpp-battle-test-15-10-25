#include "../../Framework/TestFramework.hpp"

#include <Core/Unit.hpp>
#include <Core/World.hpp>
#include <Features/Actions/AttackRanged.hpp>
#include <Features/Components/UnitProperties.hpp>

#include "../../Mocks/MockObserver.hpp"
#include "../../Mocks/MockRandom.hpp"

using namespace sw::tests;
using namespace sw::core;
using namespace sw::features::actions;
using namespace sw::features::units;

namespace
{
    std::unique_ptr<Unit> makeTarget(uint32_t id, int hp)
    {
        auto unit = std::make_unique<Unit>(id, std::vector<std::unique_ptr<IAction>>{});
        unit->setProperty<parameters::Health, properties::IHealth>(hp);
        return unit;
    }
}

SW_TEST(AttackRanged, NoDamageStat_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Attacker", { 0, 0 });
    // Unit has no Agility stat

    AttackRanged<parameters::Agility> attack(1, 3);
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackRanged, TargetOutOfMaxRange_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Agility>(4);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 4, 4 });

    AttackRanged<parameters::Agility> attack(1, 3);
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackRanged, TargetWithinRange_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Agility>(4);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 2, 2 }); // distance 2, within [1,3]

    AttackRanged<parameters::Agility> attack(1, 3);
    SW_CHECK(attack.tryAct(*world.getUnit(1), world));
    SW_CHECK(observer.contains("UnitAttacked attacker=1 target=2 damage=4"));
}

SW_TEST(AttackRanged, TargetIsBelowMinRange_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Agility>(4);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 1, 1 }); // adjacent, below minRange=2

    AttackRanged<parameters::Agility> attack(2, 3);
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackRanged, AttackBlocked_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Agility>(4);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 2, 0 });

    auto attackBlocker = [](const World&, const Point&) { return false; };
    AttackRanged<parameters::Agility> attack(1, 3, std::move(attackBlocker));
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackRanged, TargetImmuneToRanged_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Agility>(4);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });

    auto target = makeTarget(2, 10);
    target->setProperty<Immunities>(Immunities{ { AttackType::Ranged } });
    world.spawnUnit(std::move(target), "Target", { 2, 0 });

    AttackRanged<parameters::Agility> attack(1, 3);
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}
