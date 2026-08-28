#pragma once

#include <vector>

#include <Core/IAction.hpp>

namespace sw::tests
{
    // Scripted IAction:
    // - returns a fixed result
    // - counts invocations
    // - optionally appends an id to a shared external log, so tests can assert call order
    struct MockAction : public sw::core::IAction
    {
        explicit MockAction(bool result, std::vector<int>* callLog = nullptr, int id = 0)
            : _result{ result }, _callLog{ callLog }, _id{ id }
        {}

        bool tryAct(sw::core::Unit&, sw::core::World&) override
        {
            ++callCount;
            if (_callLog)
                _callLog->push_back(_id);
            return _result;
        }

        int callCount{ 0 };

    private:
        bool _result;
        std::vector<int>* _callLog;
        int _id;
    };
}
