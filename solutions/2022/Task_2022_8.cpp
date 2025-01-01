#include "Runner.h"

import std;
import utils;

namespace
{
    bool IsTreeVisible(const Array2D<char>& map, Point pos)
    {
        const char h = map[pos];
        auto check_x = [=, &map](int x) { return map[{x, pos.y}] < h; };
        auto check_y = [=, &map](int y) { return map[{pos.x, y}] < h; };
        return stdr::all_of(stdv::iota(0, pos.x), check_x)
            || stdr::all_of(stdv::iota(pos.x + 1, map.Width()), check_x)
            || stdr::all_of(stdv::iota(0, pos.y), check_y)
            || stdr::all_of(stdv::iota(pos.y + 1, map.Height()), check_y);
    }

    int ViewDistance(const Array2D<char>& map, Point pos, Direction dir)
    {
        const char h = map[pos];
        int dist = 0;
        pos = MovePoint(pos, dir);
        while (map.Contains(pos) && map[pos] < h) {
            ++dist;
            pos = MovePoint(pos, dir);
        }
        return (map.Contains(pos)) ? dist + 1 : dist;
    }

    int ScenicScore(const Array2D<char>& map, Point pos)
    {
        return ViewDistance(map, pos, Dir::N)
            * ViewDistance(map, pos, Dir::W)
            * ViewDistance(map, pos, Dir::S)
            * ViewDistance(map, pos, Dir::E);
    }

    int Solve_1(const std::filesystem::path& input)
    {
        const auto map = ReadArray2D(input);
        return stdr::count_if(to_cell_coords(map), std::bind_front(IsTreeVisible, std::cref(map)));
    }

    int Solve_2(const std::filesystem::path& input)
    {
        const auto map = ReadArray2D(input);
        int max_score = 0;
        for (Point pos : to_cell_coords(map)) {
            max_score = std::max(max_score, ScenicScore(map, pos));
        }
        return max_score;
    }

    REGISTER_SOLUTION(2022, 8, 1, Solve_1);
    REGISTER_SOLUTION(2022, 8, 2, Solve_2);
}
