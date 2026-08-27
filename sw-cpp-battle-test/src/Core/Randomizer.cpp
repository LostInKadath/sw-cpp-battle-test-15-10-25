#include <Core/Randomizer.hpp>

#include <stdexcept>

using namespace sw::core;

Randomizer::Randomizer()
    : _engine{ std::random_device{}() }
{}

size_t Randomizer::next(size_t exclusiveMax)
{
    if (exclusiveMax == 0)
        throw std::invalid_argument("exclusiveMax must be greater than 0");

    std::uniform_int_distribution<size_t> distribution(0, exclusiveMax - 1);
    return distribution(_engine);
}
