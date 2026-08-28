#include "../Framework/TestFramework.hpp"

#include <Core/Randomizer.hpp>

using namespace sw::tests;
using namespace sw::core;

SW_TEST(Randomizer, NextWithExclusiveMaxOneAlwaysReturnsZero)
{
    Randomizer random;
    for (int i = 0; i < 20; ++i)
        SW_CHECK(random.next(1) == 0);
}

SW_TEST(Randomizer, NextStaysWithinExclusiveBound)
{
    Randomizer random;
    for (int i = 0; i < 1000; ++i)
        SW_CHECK(random.next(7) < 7);
}

SW_TEST(Randomizer, NextThrowsWhenExclusiveMaxIsZero)
{
    Randomizer random;
    SW_CHECK_THROWS(random.next(0));
}
