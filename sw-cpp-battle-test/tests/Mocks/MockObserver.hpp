#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include <Core/ISimulationObserver.hpp>

namespace sw::tests
{
    // Records every reported event as a formatted string,
    // so tests can assert with simple substring checks instead of hand-rolling a struct-per-event comparison.
    struct MockObserver : public sw::core::ISimulationObserver
    {
        mutable std::vector<std::string> log;

        void onMapCreated(uint32_t tick, uint32_t width, uint32_t height) const override
        {
            record(tick,
                "MapCreated width=" + std::to_string(width) + " height=" + std::to_string(height));
        }

        void onUnitSpawned(uint32_t tick, uint32_t id, std::string_view type, const sw::core::Point& where) const override
        {
            record(tick,
                "UnitSpawned id=" + std::to_string(id) + " type=" + std::string(type) +
                " x=" + std::to_string(where.x) + " y=" + std::to_string(where.y));
        }

        void onUnitMoved(uint32_t tick, uint32_t id, const sw::core::Point& where) const override
        {
            record(tick,
                "UnitMoved id=" + std::to_string(id) + " x=" + std::to_string(where.x) + " y=" + std::to_string(where.y));
        }

        void onUnitDied(uint32_t tick, uint32_t id) const override
        {
            record(tick,
                "UnitDied id=" + std::to_string(id));
        }

        void onUnitAttacked(uint32_t tick, uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHealth) const override
        {
            record(tick,
                "UnitAttacked attacker=" + std::to_string(attackerId) + " target=" + std::to_string(targetId) +
                " damage=" + std::to_string(damage) + " targetHealth=" + std::to_string(targetHealth));
        }

        void onMarchStarted(uint32_t tick, uint32_t id, const sw::core::Point& from, const sw::core::Point& to) const override
        {
            record(tick,
                "MarchStarted id=" + std::to_string(id) +
                " from=(" + std::to_string(from.x) + "," + std::to_string(from.y) + ")" +
                " to=(" + std::to_string(to.x) + "," + std::to_string(to.y) + ")");
        }

        void onMarchEnded(uint32_t tick, uint32_t id, const sw::core::Point& where) const override
        {
            record(tick,
                "MarchEnded id=" + std::to_string(id) +
                " x=" + std::to_string(where.x) + " y=" + std::to_string(where.y));
        }

        bool contains(const std::string& substring) const
        {
            return std::ranges::any_of(log, [&substring](const auto& entry) {
                return entry.find(substring) != std::string::npos;
            });
        }

    private:
        void record(uint32_t tick, const std::string& text) const
        {
            log.push_back("[" + std::to_string(tick) + "] " + text);
        }
    };
}
