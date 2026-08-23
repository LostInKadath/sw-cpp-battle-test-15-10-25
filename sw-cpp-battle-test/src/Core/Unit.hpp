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

        uint32_t getId() const
        {
            return _id;
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

        // TODO: refactor this, try to avoid two implementations
        template <typename PropertyType, typename PropertyValueType>
        void setProperty(PropertyValueType param)
        {
            _properties.emplace<PropertyType>(std::move(param));
        }
        template <typename PropertyType>
        void setProperty()
        {
            _properties.emplace<PropertyType>();
        }

        template <typename PropertyType>
        auto getProperty() const
        {
            return _properties.get<PropertyType>();
        }

    private:
        const uint32_t _id{ 0 };

        // Unit parameters -- Health, Strength, Speed, etc.
        // Also unit properties -- MeleeTargetable, RangedTargetable, etc.
        TypeRegistry _properties;
        
        // A list of actions in a specific order. The first action that succeeds will be executed.
        std::vector<std::unique_ptr<IAction>> _actions;
    };
}
