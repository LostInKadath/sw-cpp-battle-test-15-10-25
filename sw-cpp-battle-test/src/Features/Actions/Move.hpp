#pragma once

#include <Core/IAction.hpp>
#include <Core/Point.hpp>
#include <Core/Properties.hpp>

namespace sw::features::actions
{
    template <uint32_t Speed = 1>
    struct Move : public sw::core::IAction
    {
        bool tryAct(sw::core::Unit& unit, sw::core::World& world) override
        {
            const auto march = unit.getProperty<sw::core::March>();
            if (!march)
                return false;

            auto position = world.getPosition(unit);

            for (auto step = 1; step < Speed && position != order->target; ++step)
            {
                auto nextPoint = makeStep(position, order->target);
                if (world.isCellBlocked(nextPoint))
                {
                    // According to the task, no obstacles should be considered now.
                    // We can try to occupy this cell on the next tick.
                    break;
                }
                
                world.moveUnit(unit, next);
                position = nextPoint;
                // TODO: log about movement
            }

            if (position == order->target)
            {
                unit.removeProperty<sw::core::March>();
                // TODO: log about march ended
            }

            return true;
        }

    private:
        static sw::core::Point::CoordType makeStepOneAxis(
            const sw::core::Point::CoordType& from,
            const sw::core::Point::CoordType& to)
        {
            if (from < to) return from + 1;
            if (from > to) return from - 1;
            return from;
        }

        static sw::core::Point makeStep(
            const sw::core::Point& from,
            const sw::core::Point& to)
        {
            return { makeStepOneAxis(from.x, to.x), makeStepOneAxis(from.y, to.y) };
        }
    };
}
