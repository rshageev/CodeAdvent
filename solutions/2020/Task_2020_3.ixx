export module AoC_2020.Day3;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

int Solve(const Array2D<char>& map, Point off)
{
    int trees = 0;
    Point pos{ 0, map.Height() - 1 };
    while (map.Contains(pos)) {
        pos.x = WrapCoord(pos.x + off.x, 0, map.Width());
        pos.y += off.y;
        if (map[pos] == '#') {
            ++trees;
        }
    }
    return trees;
}

int64 Solve_1(const std::filesystem::path& input)
{
    const auto map = ReadArray2D(input);
    return Solve(map, { 3,-1 });
}

int64 Solve_2(const std::filesystem::path& input)
{
    const auto map = ReadArray2D(input);
    int64 res = 1;
    const Point slopes[] = { { 1, -1 }, { 3,-1 }, { 5,-1 }, { 7, -1 }, { 1, -2 } };
    for (Point off : slopes) {
        res *= Solve(map, off);
    }
    return res;
}