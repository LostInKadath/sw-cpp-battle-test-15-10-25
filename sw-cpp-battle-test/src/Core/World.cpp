#include <Core/World.hpp>

#include <type_traits>

using namespace sw::core;

World::World(uint32_t width, uint32_t height, const ISimulationObserver& observer)
    : _width{ width }, _height{ height }, _observer{ observer }
{
    _observer.onMapCreated(_currentTick, _width, _height);
}

void World::spawnUnit(std::unique_ptr<Unit>&& unit, std::string_view type, const Point& position)
{
    if (!unit)
        throw std::runtime_error("No unit provided!");

    const auto id = unit->getId();
    if (_units.contains(id))
        throw std::runtime_error("Unit with id " + std::to_string(id) + " already exists!");

    if (!isPositionOnMap(position))
        throw std::runtime_error("Position " + std::to_string(position.x) + ", " + std::to_string(position.y) + " is beyond map limits!");

    if (isCellBlocked(position))
        throw std::runtime_error("Position " + std::to_string(position.x) + ", " + std::to_string(position.y) + " is already occupied!");

    _units.emplace(id, std::move(unit));
    _positions.emplace(id, position);
    _turnOrder.push_back(id);

    _observer.onUnitSpawned(_currentTick, id, type, position);
}

std::unique_ptr<Unit>& World::getUnit(UnitId id)
{
    return _units.at(id);
}

Point World::getPosition(const Unit& unit) const
{
    return _positions.at(unit.getId());
}

bool World::isCellBlocked(const Point& position) const
{
    if (!isPositionOnMap(position))
        throw std::runtime_error("Position " + std::to_string(position.x) + ", " + std::to_string(position.y) + " is beyond map limits!");

    constexpr const auto MinRange = 0;
    constexpr const auto MaxRange = 0;

    auto conflictingUnits = findUnits(position, MinRange, MaxRange, [](const Unit& unit) {
        return !unit.getProperty<properties::NotOccupies>();
    });
    return !conflictingUnits.empty();
}

std::vector<Unit*> World::findUnits(
    const Point& position,
    size_t minRange,
    size_t maxRange,
    const std::function<bool(const Unit&)>& filter) const
{
    std::vector<Unit*> targets;

    // TODO: could loop inside a square? Might be faster than checking all units.
    for (const auto& [id, unit] : _units)
    {
        const auto& unitPosition = _positions.at(id);
        const auto distance = getDistance(unitPosition, position);

        if (minRange <= distance && distance <= maxRange && filter(*unit))
            targets.push_back(unit.get());
    }

    return targets;
}

void World::moveUnit(const Unit& unit, const Point& position)
{
    if (!isPositionOnMap(position))
        throw std::runtime_error("Position " + std::to_string(position.x) + ", " + std::to_string(position.y) + " is beyond map limits!");

    const auto id = unit.getId();
    _positions.at(id) = position;
    _observer.onUnitMoved(_currentTick, id, position);
}

void World::startMarch(uint32_t unitId, const Point& to)
{
    if (!isPositionOnMap(to))
        throw std::runtime_error("Position " + std::to_string(to.x) + ", " + std::to_string(to.y) + " is beyond map limits!");

    auto& unit = _units.at(unitId);
    unit->setProperty<core::properties::March>(to);

    const auto from =_positions.at(unitId);
    _observer.onMarchStarted(_currentTick, unitId, from, to);
}

void World::completeMarch(Unit& unit)
{
    unit.removeProperty<properties::March>();
    _observer.onMarchEnded(_currentTick, unit.getId(), _positions.at(unit.getId()));
}

void World::applyDamage(Unit& attacker, Unit& target, uint32_t amount)
{
    uint32_t targetHealth = 0;
    if (auto health = target.getProperty<properties::IHealth>())
    {
        health->changeValue(-static_cast<int>(amount));
        targetHealth = health->currentValue();
    }
    _observer.onUnitAttacked(_currentTick, attacker.getId(), target.getId(), amount, targetHealth);
}

void World::healUnit(Unit& healer, Unit& target, uint32_t amount)
{
    uint32_t targetHealth = 0;
    if (auto health = target.getProperty<properties::IHealth>())
    {
        health->changeValue(+static_cast<int>(amount));
        targetHealth = health->currentValue();
    }
    //_observer.onUnitHealed(_currentTick, healer.getId(), target.getId(), amount, targetHealth);
}

bool World::step(uint32_t tick)
{
    _currentTick = tick;

    bool anyActed = false;
    for (auto id : _turnOrder)      // Units act in order of creation
    {
        auto it = _units.find(id);
        if (it != _units.end() && it->second->tryAct(*this))
            anyActed = true;
    }

    // Finalize the step, remove dead units
    for (auto it = _units.begin(); it != _units.end(); )
    {
        auto health = it->second->getProperty<properties::IHealth>();
        if (!health || !health->isDead())
        {
            ++it;
            continue;
        }

        _observer.onUnitDied(_currentTick, it->first);

        _positions.erase(it->first);
        std::erase(_turnOrder, it->first);
        it = _units.erase(it);
    }

    return _units.size() > 1 && anyActed;
}

bool World::isPositionOnMap(const Point& p) const
{
    return 0 <= p.x && p.x < _width &&
            0 <= p.y && p.y < _height;
}
