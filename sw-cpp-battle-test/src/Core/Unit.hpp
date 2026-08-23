#pragma once

#include <memory>
#include <vector>

#include "Core/IAction.hpp"
#include "IO/System/TypeRegistry.hpp"

namespace sw::core
{
    struct Unit
    {
        explicit Unit(uint32_t id, std::vector<std::unique_ptr<IAction>> actions)
            : _id{ id }, _actions{ std::move(actions) }
        {
        }

        virtual ~Unit() = default;

        /** Makes a move for a unit.
         * Tries all registered actions in order and executes the first one that succeeds.
         * Returns true if an action was executed, false otherwise.
         */
        bool act(World& world)
        {
            for (auto& action : _actions)
            {
                if (action->tryAct(*this, world))
                    return true;
            }
            return false;
        }

        template <typename ParamType>
        auto getParam() const
        {
            return _params.get<ParamType>();
        }

    private:
        const uint32_t _id{ 0 };

        // Health, Strength, Speed, etc.
        TypeRegistry _params;
        
        // A list of actions in a specific order. The first action that succeeds will be executed.
        std::vector<std::unique_ptr<IAction>> _actions;
    };
}
