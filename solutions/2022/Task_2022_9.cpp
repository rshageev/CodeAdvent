#include "Runner.h"

import std;
import utils;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<std::pair<Direction, int>> directions;
        for (std::string_view line : ReadLines(input)) {
            directions.emplace_back(CharToDir(line[0], "LRUD"), to_int(line.substr(2)));
        }
        return directions;
    }

    Point MoveTail(Point head, Point tail)
    {
        Point off = head - tail;
        if (std::abs(off.x) <= 1 && std::abs(off.y) <= 1)
            return tail;

        return {
            tail.x + ((off.x > 0) - (off.x < 0)),
            tail.y + ((off.y > 0) - (off.y < 0))
        };
    }

    void MoveRope(std::span<Point> rope, Direction dir)
    {
        rope[0] = MovePoint(rope[0], dir);
        for (auto&& [head, tail] : rope | stdv::pairwise) {
            tail = MoveTail(head, tail);
        }
    }

    auto Solve(int rope_length, const std::filesystem::path& input)
    {
        std::vector<Point> rope(rope_length, Point{ 0,0 });
        std::unordered_set<Point> visited = { {0, 0} };
        for (auto [dir, dist] : LoadData(input)) {
            for (int i = 0; i < dist; ++i) {
                MoveRope(rope, dir);
                visited.insert(rope.back());
            }
        }
        return visited.size();
    }

    REGISTER_SOLUTION(2022, 9, 1, std::bind_front(Solve, 2));
    REGISTER_SOLUTION(2022, 9, 2, std::bind_front(Solve, 10));
}
