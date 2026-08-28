#pragma once

#include <cstddef>
#include <vector>

#include <Core/IRandom.hpp>

namespace sw::tests
{
    // Returns a fixed value, or walks a scripted sequence (repeating)
    class MockRandom : public sw::core::IRandom
    {
    public:
        explicit MockRandom(size_t fixedValue = 0)
            : _sequence{ fixedValue }
        {}

        explicit MockRandom(std::vector<size_t> sequence)
            : _sequence{ std::move(sequence) }
        {}

        size_t next(size_t exclusiveMax) override
        {
            ++_callCount;
            _lastExclusiveMax = exclusiveMax;

            const auto value = _sequence[_index % _sequence.size()];
            ++_index;
            return value;
        }

        size_t callCount() const { return _callCount; }
        size_t lastExclusiveMax() const { return _lastExclusiveMax; }

    private:
        std::vector<size_t> _sequence;
        size_t _index{ 0 };
        size_t _callCount{ 0 };
        size_t _lastExclusiveMax{ 0 };
    };
}
