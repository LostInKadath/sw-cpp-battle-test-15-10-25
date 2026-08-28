#include "../../Framework/TestFramework.hpp"

#include <Core/Unit.hpp>
#include <Core/World.hpp>
#include <Features/Actions/AttackMelee.hpp>
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

SW_TEST(AttackMelee, NoDamageStat_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    world.spawnUnit(std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{}), "Attacker", { 0, 0 });
    // Unit has no Strength stat

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackMelee, NoTargetInRange_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Strength>(5);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 5, 5 }); // out of melee range

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackMelee, TargetInRangeX_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Strength>(5);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 1, 0 });

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(attack.tryAct(*world.getUnit(1), world));
    SW_CHECK(observer.contains("UnitAttacked attacker=1 target=2 damage=5"));
}

SW_TEST(AttackMelee, TargetInRangeY_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Strength>(5);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 0, 1 });

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(attack.tryAct(*world.getUnit(1), world));
    SW_CHECK(observer.contains("UnitAttacked attacker=1 target=2 damage=5"));
}

SW_TEST(AttackMelee, TargetInRangeXY_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Strength>(5);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 1, 1 });

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(attack.tryAct(*world.getUnit(1), world));
    SW_CHECK(observer.contains("UnitAttacked attacker=1 target=2 damage=5"));
}

SW_TEST(AttackMelee, TargetImmuneToMelee_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Strength>(5);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });

    auto target = makeTarget(2, 10);
    target->setProperty<Immunities>(Immunities{ { AttackType::Melee } });
    world.spawnUnit(std::move(target), "Target", { 1, 0 });

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackMelee, DeadTarget_ReturnsFalse)
{
    MockObserver observer;
    MockRandom random;
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Strength>(5);
    world.spawnUnit(std::move(attacker), "Attacker", { 0, 0 });
    world.spawnUnit(makeTarget(2, 0), "Target", { 1, 0 }); // already at 0 HP

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(!attack.tryAct(*world.getUnit(1), world));
}

SW_TEST(AttackMelee, MultipleTargetsAreInRange_UseRandomizer_ReturnsTrue)
{
    MockObserver observer;
    MockRandom random(1);   // every next() returns 1
    World world(10, 10, observer, random);
    auto attacker = std::make_unique<Unit>(1, std::vector<std::unique_ptr<IAction>>{});
    attacker->setProperty<parameters::Strength>(5);
    world.spawnUnit(std::move(attacker), "Attacker", { 5, 5 });
    world.spawnUnit(makeTarget(2, 10), "Target", { 5, 6 });
    world.spawnUnit(makeTarget(3, 10), "Target", { 6, 5 });

    AttackMelee<parameters::Strength> attack;
    SW_CHECK(attack.tryAct(*world.getUnit(1), world));
    SW_CHECK(random.callCount() == 1);
    SW_CHECK(random.lastExclusiveMax() == 2);   // choosing between two targets
}
