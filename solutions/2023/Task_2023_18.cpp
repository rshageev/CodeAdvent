#include "Runner.h"

import std;
import utils;

namespace
{
    using Point64 = TPoint<int64>;

    std::vector<Point64> GeneratePolygon(auto&& ops)
    {
        std::vector<Point64> path = { {0, 0} };
        for (auto [dir, dist] : ops) {
            path.push_back(MovePoint(path.back(), dir, dist));
        }
        return path;
    }

    int64 CalcArea(auto&& points)
    {
        // uses Shoelace formula

        int64 sum = 0;
        int64 len = 0;
        for (auto [p1, p2] : points | stdv::pairwise)
        {
            sum += p1.x * p2.y - p1.y * p2.x;
            len += std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
        }
        return std::abs(sum) / 2 + len / 2 + 1;
    }

    std::pair<Direction, int64> ParseOp1(std::string_view str)
    {
        auto dir = CharToDir(str[0], "LRUD");

        str.remove_prefix(2);
        auto dist = Read<int64>(str);

        return { dir, dist };
    }

    std::pair<Direction, int64> ParseOp2(std::string_view str)
    {
        str = str.substr(str.size() - 7, 6);

        auto dir = CharToDir(str.back(), "2031");

        str.remove_suffix(1);
        auto dist = Read<int64, 16>(str);

        return { dir, dist };
    }

    int64 Solve(const std::filesystem::path& input, auto&& parse)
    {
        auto ops = ReadLines(input) | stdv::transform(parse);
        const auto points = GeneratePolygon(ops);
        return CalcArea(points);
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        return Solve(input, ParseOp1);
    }

    int64 Solve_2(const std::filesystem::path& input)
    {
        return Solve(input, ParseOp2);
    }

    REGISTER_SOLUTION(2023, 18, 1, Solve_1);
    REGISTER_SOLUTION(2023, 18, 2, Solve_2);
}