#include "Simulation.hpp"

using namespace sw::core;

Simulation::Simulation(ISimulationObserver& observer)
    : _observer{ observer }
{}

void Simulation::createMap(uint32_t width, uint32_t height)
{
    _world.emplace(width, height, _observer, _random);
}

void Simulation::spawnSwordsman(uint32_t unitId, uint32_t x, uint32_t y, uint32_t health, uint32_t strength)
{
    spawnUnit<features::units::Swordsman>(unitId, x, y, health, strength);
}

void Simulation::spawnHunter(uint32_t unitId, uint32_t x, uint32_t y, uint32_t health, uint32_t agility, uint32_t strength, uint32_t range)
{
    spawnUnit<features::units::Hunter>(unitId, x, y, health, agility, strength, range);
}

void Simulation::marchUnit(uint32_t unitId, uint32_t targetX, uint32_t targetY)
{
    _world->startMarch(unitId, {targetX, targetY});
}

void Simulation::run()
{
    uint32_t tick = 1;
    while (_world->step(tick))
        ++tick;
}
