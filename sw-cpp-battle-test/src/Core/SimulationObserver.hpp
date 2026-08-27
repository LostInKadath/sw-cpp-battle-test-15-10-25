#pragma once

#include <IO/System/EventLog.hpp>

#include <Core/ISimulationObserver.hpp>

namespace sw::simulation
{
    struct SimulationObserver : public sw::core::ISimulationObserver
    {
        SimulationObserver(EventLog&);

        void onMapCreated(uint32_t tick, uint32_t width, uint32_t height) const override;

        void onUnitSpawned(uint32_t tick, uint32_t id, std::string_view type, const sw::core::Point& where) const override;
        void onUnitMoved(uint32_t tick, uint32_t id, const sw::core::Point& where) const override;
        void onUnitDied(uint32_t tick, uint32_t id) const override;
        void onUnitAttacked(uint32_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHealth) const override;

        void onMarchStarted(uint32_t tick, uint32_t id, const sw::core::Point& from, const sw::core::Point& to) const override;
        void onMarchEnded(uint32_t tick, uint32_t id, const sw::core::Point& where) const override;

    private:
        EventLog& _eventLog;
    };
}
