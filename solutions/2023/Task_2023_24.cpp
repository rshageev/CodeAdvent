#include "Runner.h"

import std;
import utils;

namespace
{
    using Point64 = TPoint<std::int64_t>;
    using Point64d = TPoint<double>;

    struct Stone
    {
        Point64 p;
        std::int64_t pz;
        Point64 v;
        std::int64_t vz;
    };

    Stone ParseLine(std::string_view str)
    {
        auto x = Read<std::int64_t>(str); Skip(str, ", "); while (str.starts_with(' ')) str.remove_prefix(1);
        auto y = Read<std::int64_t>(str); Skip(str, ", "); while (str.starts_with(' ')) str.remove_prefix(1);
        auto z = Read<std::int64_t>(str); Skip(str, " @ "); while (str.starts_with(' ')) str.remove_prefix(1);
        auto vx = Read<std::int64_t>(str); Skip(str, ", "); while (str.starts_with(' ')) str.remove_prefix(1);
        auto vy = Read<std::int64_t>(str); Skip(str, ", "); while (str.starts_with(' ')) str.remove_prefix(1);
        auto vz = Read<std::int64_t>(str);
        return Stone{ .p = {x,y}, .pz = z, .v = { vx, vy }, .vz = vz };
    }

    auto LoadInput(const std::filesystem::path& input)
    {
        std::vector<Stone> stones;
        for (const auto& line : ReadLines(input) | stdv::filter(not_empty)) {
            stones.push_back(ParseLine(line));
        }
        return stones;
    }

    std::optional<Point64d> RayIntersection(Point64 p1, Point64 v1, Point64 p2, Point64 v2)
    {
        auto dx = p2.x - p1.x;
        auto dy = p2.y - p1.y;
        auto det = v2.x * v1.y - v2.y * v1.x;

        if (det == 0) {
            // parallel
            return std::nullopt;
        }

        //double u = static_cast<double>(dy * v2.x - dx * v2.y) / (double)det;
        //double v = static_cast<double>(dy * v1.x - dx * v1.y) / (double)det;

        double t1 = double(v2.x * dy - v2.y * dx) / det;
        double t2 = double(v1.x * dy - v1.y * dx) / det;

        double cx1 = v1.x * t1 + p1.x;
        double cy1 = v1.y * t1 + p1.y;

        double cx2 = v2.x * t2 + p2.x;
        double cy2 = v2.y * t2 + p2.y;

        if (t1 >= 0.0 && t2 >= 0.0)
        {
            double cx = v1.x * t1 + p1.x;
            double cy = v1.y * t1 + p1.y;
            return Point64d{ cx, cy };
        }

        return std::nullopt;
    }

    std::int64_t CountIntersections(std::span<const Stone> stones, std::int64_t area_min, std::int64_t area_max)
    {
        const auto offset = area_min;
        area_min = 0;
        area_max -= offset;

        std::int64_t count = 0;
        for (size_t i = 1; i < stones.size(); ++i)
        {
            for (size_t j = 0; j < i; ++j)
            {
                auto p1 = stones[i].p;
                auto v1 = stones[i].v;
                auto p2 = stones[j].p;
                auto v2 = stones[j].v;

                p1.x -= offset;
                p1.y -= offset;
                p2.x -= offset;
                p2.y -= offset;

                if (auto cp = RayIntersection(p1, v1, p2, v2))
                {
                    if (cp->x >= area_min && cp->x <= area_max &&
                        cp->y >= area_min && cp->y <= area_max)
                    {
                        ++count;
                    }
                }
            }
        }
        return count;
    }

    std::int64_t Solve_1(const std::filesystem::path& input)
    {
        constexpr std::int64_t MinLimit = 200000000000000;
        constexpr std::int64_t MaxLimit = 400000000000000;

        const auto stones = LoadInput(input);

        return CountIntersections(stones, MinLimit, MaxLimit);
    }

    REGISTER_SOLUTION(2023, 24, 1, Solve_1);
}