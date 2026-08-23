#pragma once

#include <string>

#include <Core/Point.hpp>

namespace sw::core
{
    struct ISimulationObserver
    {
        virtual ~ISimulationObserver() = default;

        virtual void onMapCreated(uint32_t tick, uint32_t width, uint32_t height) const = 0;

        virtual void onUnitSpawned(uint32_t tick, uint32_t id, const std::string& type, const Point& where) const = 0;
        virtual void onUnitMoved(uint32_t tick, uint32_t id, const Point& where) const = 0;
        virtual void onUnitDied(uint32_t tick, uint32_t id) const = 0;
        virtual void onUnitAttacked(uint32_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHealth) const = 0;

        virtual void onMarchStarted(uint32_t tick, uint32_t id, const Point& from, const Point& to) const = 0;
        virtual void onMarchEnded(uint32_t tick, uint32_t id, const Point& where) const = 0;
    };
}
