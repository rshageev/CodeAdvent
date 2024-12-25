#include "Runner.h"

import std;
import utils;

namespace
{
    bool IsMatch(const Array2D<char>& data, Point base_pos, Point dir)
    {
        constexpr std::string_view xmas = "XMAS";
        for (int i = 0; i < 4; ++i) {
            Point pos = base_pos + dir * i;
            if (!data.Contains(pos) || data[pos] != xmas[i]) {
                return false;
            }
        }
        return true;
    }

    int Search(const Array2D<char>& data)
    {
        constexpr Point Dirs[8] = { {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1} };
        int result = 0;
        for (Point pos : to_cell_coords(data)) {
            for (Point dir : Dirs) {
                if (IsMatch(data, pos, dir)) {
                    ++result;
                }
            }
        }

        return result;
    }

    bool IsMatchX_OneDir(const Array2D<char>& data, Point base_pos, Point dir)
    {
        constexpr std::string_view xmas = "MAS";
        for (int i = -1; i <= 1; ++i) {
            Point pos = base_pos + dir * i;
            if (!data.Contains(pos) || data[pos] != xmas[i + 1]) {
                return false;
            }
        }
        return true;
    }

    bool IsMatchX(const Array2D<char>& data, Point base_pos)
    {
        constexpr Point Dirs[4] = { {1,1}, {-1,1}, {-1,-1}, {1,-1} };
        int dirs_matched = 0;
        for (Point dir : Dirs) {
            if (IsMatchX_OneDir(data, base_pos, dir)) {
                ++dirs_matched;
            }
        }
        return dirs_matched == 2;
    }

    int SearchX(const Array2D<char>& data)
    {
        int result = 0;
        for (Point pos : to_cell_coords(data)) {
            if (IsMatchX(data, pos)) {
                ++result;
            }
        }
        return result;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        return Search(ReadArray2D(input));
    }

    int Solve_2(const std::filesystem::path& input)
    {
        return SearchX(ReadArray2D(input));
    }

    REGISTER_SOLUTION(2024, 4, 1, Solve_1);
    REGISTER_SOLUTION(2024, 4, 2, Solve_2);
}