#pragma once

#include <IO/System/EventLog.hpp>

#include <Core/ISimulationObserver.hpp>

namespace sw::simulation
{
    struct SimulationObserver : public sw::core::ISimulationObserver
    {
        SimulationObserver(EventLog& log)
            : _eventLog{ log }
        {}

        void onMapCreated(uint32_t tick, uint32_t width, uint32_t height) const
        {
            _eventLog.log(tick, io::MapCreated{width, height});
        }

        void onUnitSpawned(uint32_t tick, uint32_t id, std::string_view type, const sw::core::Point& where) const
        {
            _eventLog.log(tick, io::UnitSpawned{id, std::string{ type }, where.x, where.y});
        }

        void onUnitMoved(uint32_t tick, uint32_t id, const sw::core::Point& where) const
        {
            _eventLog.log(tick, io::UnitMoved{id, where.x, where.y});
        }

        void onUnitDied(uint32_t tick, uint32_t id) const
        {
            _eventLog.log(tick, io::UnitDied{id});
        }

        void onUnitAttacked(uint32_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHealth) const
        {
            _eventLog.log(tick, io::UnitAttacked{attackerId, targetId, damage, targetHealth});
        }

        void onMarchStarted(uint32_t tick, uint32_t id, const sw::core::Point& from, const sw::core::Point& to) const
        {
            _eventLog.log(tick, io::MarchStarted{id, from.x, from.y, to.x, to.y});
        }

        void onMarchEnded(uint32_t tick, uint32_t id, const sw::core::Point& where) const
        {
            _eventLog.log(tick, io::MarchEnded{id, where.x, where.y});
        }

    private:
        EventLog& _eventLog;
    };
}
