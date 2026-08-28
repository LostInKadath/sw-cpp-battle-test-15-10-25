cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_SOURCES src/*.cpp src/*.hpp)
list(REMOVE_ITEM LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(sw_battle_test src/main.cpp ${LIB_SOURCES})
target_include_directories(sw_battle_test PUBLIC src/)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp tests/*.hpp)
add_executable(sw_battle_test_tests ${TEST_SOURCES} ${LIB_SOURCES})
target_include_directories(sw_battle_test_tests PUBLIC src/ tests/)

enable_testing()
add_test(NAME AllTests COMMAND sw_battle_test_tests)
