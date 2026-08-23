#pragma once

namespace sw::core
{
    struct World
    {
        World(int width, int height)
            : _width{ width }, _height{ height }
        {
        }

    private:
        int _width{ 0 };
        int _height{ 0 };
    };
}
