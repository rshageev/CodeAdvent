#include "Runner.h"

import std;
import utils;

namespace
{
    using Point64 = TPoint<int64>;
    using Rect64 = TRect<int64>;

    constexpr Rect64 ToRect(Point64 p1, Point64 p2) {
        auto [minx, maxx] = std::minmax({ p1.x, p2.x });
        auto [miny, maxy] = std::minmax({ p1.y, p2.y });
        return { .x = minx, .y = miny, .w = (maxx - minx + 1), .h = (maxy - miny + 1) };
    }

    auto ReadData(const std::filesystem::path& input)
    {
        std::vector<Point64> pts;
        for (const auto line : ReadLines(input)) {
            auto [s1, s2] = Split2(line, ',');
            pts.emplace_back(to_int64(s1), to_int64(s2));
        }
        return pts;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        const auto points = ReadData(input);
        int64 max_area = 0;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                auto rect = ToRect(points[i], points[j]);
                max_area = std::max(max_area, rect.w * rect.h);
            }
        }

        return max_area;
    }

    constexpr bool IsInside(std::span<const Point64> polygon, Rect64 rect)
    {
        for (auto [e1, e2] : polygon | stdv::pairwise) {
            if (Intersects(rect, ToRect(e1, e2)))
                return false;
        }
        return true;
    }

    int64 Solve_2(const std::filesystem::path& input)
    {
        auto points = ReadData(input);

        auto polygon = points;
        polygon.push_back(points[0]);

        int64 max_area = 0;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                auto rect = ToRect(points[i], points[j]);
                auto area = rect.w * rect.h;
                if (area > max_area && IsInside(polygon, rect)) {
                    max_area = area;
                }
            }
        }
        return max_area;
    }

    REGISTER_SOLUTION(2025, 9, 1, Solve_1);
    REGISTER_SOLUTION(2025, 9, 2, Solve_2);
}
