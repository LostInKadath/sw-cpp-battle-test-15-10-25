#pragma once

#include <cstddef>

namespace sw::core
{
    struct IRandom
    {
        virtual ~IRandom() = default;

        // Returns a random value in [0, exclusiveMax).
        virtual size_t next(size_t exclusiveMax) = 0;
    };
}
