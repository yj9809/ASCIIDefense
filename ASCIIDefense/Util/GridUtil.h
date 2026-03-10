#pragma once

#include "Math/Vector2.h"

#include <vector>

namespace GridUtil
{
    void Fill3x3(std::vector<std::vector<int>>& grid, const Wanted::Vector2& center, int value)
    {
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx)
                grid[center.y + dy][center.x + dx] = value;
    }

    void Clear3x3(std::vector<std::vector<int>>& grid, const Wanted::Vector2& center)
    {
        Fill3x3(grid, center, 0);
    }

    bool IsInside3x3(const Vector2& center, const Vector2& p)
    {
        return p.x >= center.x - 1 && p.x <= center.x + 1
            && p.y >= center.y - 1 && p.y <= center.y + 1;
    }
}