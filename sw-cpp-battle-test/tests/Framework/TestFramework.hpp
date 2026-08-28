#pragma once

#include <functional>
#include <string>
#include <vector>

namespace sw::tests
{
    struct AssertionFailure
    {
        std::string message;
    };

    struct TestCase
    {
        std::string group;
        std::string name;
        std::function<void()> body;
    };

    inline std::vector<TestCase>& registry()
    {
        static std::vector<TestCase> tests;
        return tests;
    }

    struct TestRegistry
    {
        TestRegistry(std::string group, std::string name, std::function<void()> body)
        {
            registry().push_back({ std::move(group), std::move(name), std::move(body) });
        }
    };

    int runAll();
}

#define SW_TEST(group, name) \
    static void sw_test_##group##_##name(); \
    static sw::tests::TestRegistry sw_reg_##group##_##name(#group, #name, sw_test_##group##_##name); \
    static void sw_test_##group##_##name()

#define SW_CHECK(expr) \
    do { \
        if (!(expr)) \
            throw sw::tests::AssertionFailure{ std::string(__FILE__) + ":" + std::to_string(__LINE__) + "  CHECK(" #expr ")" }; \
    } while (false)

#define SW_CHECK_THROWS(expr) \
    do { \
        bool sw_threw = false; \
        try { (void)(expr); } catch (...) { sw_threw = true; } \
        if (!sw_threw) \
            throw sw::tests::AssertionFailure{ std::string(__FILE__) + ":" + std::to_string(__LINE__) + "  CHECK_THROWS(" #expr ") -- nothing was thrown" }; \
    } while (false)
