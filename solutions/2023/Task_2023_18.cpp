#include "Runner.h"

import std;
import utils;

namespace
{
    using Point64 = TPoint<std::int64_t>;

    std::vector<Point64> GeneratePolygon(auto&& ops)
    {
        std::vector<Point64> path = { {0, 0} };
        for (auto [dir, dist] : ops) {
            path.push_back(MovePoint(path.back(), dir, dist));
        }
        return path;
    }

    std::int64_t CalcArea(auto&& points)
    {
        std::int64_t sum = 0;
        std::int64_t len = 0;
        for (auto [p1, p2] : points | stdv::pairwise)
        {
            sum += p1.x * p2.y - p1.y * p2.x;
            len += std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
        }
        return std::abs(sum) / 2 + len / 2 + 1;
    }

    std::pair<Direction, std::int64_t> ParseOp1(std::string_view str)
    {
        auto dir = CharToDir(str[0], "LRUD");

        str.remove_prefix(2);
        auto dist = Read<std::int64_t>(str);

        return { dir, dist };
    }

    std::pair<Direction, std::int64_t> ParseOp2(std::string_view str)
    {
        str = str.substr(str.size() - 7, 6);

        auto dir = CharToDir(str.back(), "2031");

        str.remove_suffix(1);
        auto dist = Read<std::int64_t, 16>(str);

        return { dir, dist };
    }

    std::int64_t Solve(const std::filesystem::path& input, auto&& parse)
    {
        auto ops = ReadLines(input) | stdv::transform(parse);
        const auto points = GeneratePolygon(ops);
        return CalcArea(points);
    }

    std::int64_t Solve_1(const std::filesystem::path& input)
    {
        return Solve(input, ParseOp1);
    }

    std::int64_t Solve_2(const std::filesystem::path& input)
    {
        return Solve(input, ParseOp2);
    }

    REGISTER_SOLUTION(2023, 18, 1, Solve_1);
    REGISTER_SOLUTION(2023, 18, 2, Solve_2);
}