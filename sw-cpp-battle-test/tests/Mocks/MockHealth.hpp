#pragma once

#include <Core/Properties.hpp>

namespace sw::tests
{
    struct MockHealth : public sw::core::properties::IHealth
    {
        int value;
        
        explicit MockHealth(int v) : value(v) {}

        bool isDead() const override { return value <= 0; }
        int currentValue() const override { return value; }
        void changeValue(int delta) override { value += delta; }
    };
}
