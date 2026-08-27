#include <Core/SimulationObserver.hpp>

#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>

using namespace sw::simulation;

SimulationObserver::SimulationObserver(sw::EventLog& log)
    : _eventLog{ log }
{}

void SimulationObserver::onMapCreated(uint32_t tick, uint32_t width, uint32_t height) const
{
    _eventLog.log(tick, sw::io::MapCreated{width, height});
}

void SimulationObserver::onUnitSpawned(uint32_t tick, uint32_t id, std::string_view type, const sw::core::Point& where) const
{
    _eventLog.log(tick, sw::io::UnitSpawned{id, std::string{ type }, where.x, where.y});
}

void SimulationObserver::onUnitMoved(uint32_t tick, uint32_t id, const sw::core::Point& where) const
{
    _eventLog.log(tick, sw::io::UnitMoved{id, where.x, where.y});
}

void SimulationObserver::onUnitDied(uint32_t tick, uint32_t id) const
{
    _eventLog.log(tick, sw::io::UnitDied{id});
}

void SimulationObserver::onUnitAttacked(uint32_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHealth) const
{
    _eventLog.log(tick, sw::io::UnitAttacked{attackerId, targetId, damage, targetHealth});
}

void SimulationObserver::onMarchStarted(uint32_t tick, uint32_t id, const sw::core::Point& from, const sw::core::Point& to) const
{
    _eventLog.log(tick, sw::io::MarchStarted{id, from.x, from.y, to.x, to.y});
}

void SimulationObserver::onMarchEnded(uint32_t tick, uint32_t id, const sw::core::Point& where) const
{
    _eventLog.log(tick, sw::io::MarchEnded{id, where.x, where.y});
}
