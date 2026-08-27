#pragma once

#include <random>

#include <Core/IRandom.hpp>

namespace sw::core
{
    class Randomizer : public IRandom
    {
    public:
        Randomizer();

        size_t next(size_t exclusiveMax) override;

    private:
        std::mt19937 _engine;
    };
}
