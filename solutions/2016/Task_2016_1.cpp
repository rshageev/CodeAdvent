#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {     
        const auto text = ReadText(input);
        std::string_view str = text;
        std::vector<std::pair<Direction, int>> directions;
        Direction dir = Dir::N;
        while (!str.empty()) {
            dir = (ReadChar(str) == 'L') ? RotateLeft(dir) : RotateRight(dir);
            directions.emplace_back(dir, Read<int>(str));
            str.remove_prefix(std::min<size_t>(2, str.size()));
        }
        return directions;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        Point pos{ 0,0 };
        for (auto [dir, dist] : LoadData(input)) {
            pos = MovePoint(pos, dir, dist);
        }
        return MDist(pos);
    }

    int Solve_2(const std::filesystem::path& input)
    {
        Point pos{ 0,0 };
        std::unordered_set<Point> visited = { pos };
        for (auto [dir, dist] : LoadData(input)) {
            for (int i = 0; i < dist; ++i) {
                pos = MovePoint(pos, dir, 1);
                if (!visited.insert(pos).second) {
                    return MDist(pos);
                }
            }
        }
        return 0;
    }

    REGISTER_SOLUTION(2016, 1, 1, Solve_1);
    REGISTER_SOLUTION(2016, 1, 2, Solve_2);
}
