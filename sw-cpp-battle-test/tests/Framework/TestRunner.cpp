#include "TestFramework.hpp"

#include <iostream>

namespace sw::tests
{
    int runAll()
    {
        int passed = 0;
        int failed = 0;

        for (auto& test : registry())
        {
            try
            {
                test.body();
                ++passed;
            }
            catch (const AssertionFailure& failure)
            {
                ++failed;
                std::cout << "[FAIL] " << test.group << "." << test.name << " -- " << failure.message << "\n";
            }
            catch (const std::exception& e)
            {
                ++failed;
                std::cout << "[FAIL] " << test.group << "." << test.name << " -- unexpected exception: " << e.what() << "\n";
            }
            catch (...)
            {
                ++failed;
                std::cout << "[FAIL] " << test.group << "." << test.name << " -- unknown exception\n";
            }
        }

        std::cout << "\n" << passed << " passed, " << failed << " failed, " << registry().size() << " total\n";
        return failed == 0 ? 0 : 1;
    }
}
