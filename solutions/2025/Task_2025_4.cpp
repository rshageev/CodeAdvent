#include "Runner.h"

import std;
import utils;

namespace
{
    int CanAccess(const Array2D<bool>& data, Point pos)
    {
        int count = 0;
        for (auto [_, off] : Neighbours8) {
            Point npos = pos + off;
            if (data.Contains(npos) && data[npos]) {
                ++count;
            }
        }
        return (count < 4);
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        auto data = ReadArray2D(input, equal<'@'>);

        return stdr::count_if(to_cell_coords(data), [&](Point pos) {
            return data[pos] && CanAccess(data, pos);
        });
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        auto data = ReadArray2D(input, equal<'@'>);

        int removeCount = 0;
        bool removedAny = true;
        while (removedAny) {
            removedAny = false;
            for (Point pos : to_cell_coords(data)) {
                if (data[pos] && CanAccess(data, pos)) {
                    data[pos] = false;
                    removedAny = true;
                    ++removeCount;
                }
            }
        }
        return removeCount;
    }

    REGISTER_SOLUTION(2025, 4, 1, Solve_1);
    REGISTER_SOLUTION(2025, 4, 2, Solve_2);
}
