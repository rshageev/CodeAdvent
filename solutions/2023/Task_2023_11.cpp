#include "Runner.h"

import std;
import utils;

namespace
{
    std::vector<Point> LoadStarPositions(const std::filesystem::path& input)
    {
        const auto map = ReadArray2D(input, equal<'#'>);

        std::vector<Point> stars;
        for (Point pos : to_cell_coords(map)) {
            if (map[pos]) stars.push_back(pos);
        }
        return stars;
    }

    void Expand(std::span<Point> stars, int factor)
    {
        auto expand = [=](int Point::* coord) {
            stdr::sort(stars, std::less{}, coord);
            for (int offset = 0; auto [s1, s2] : stars | stdv::pairwise) {
                offset += std::max(0, (offset + s2.*coord - s1.*coord - 1)) * (factor - 1);
                s2.*coord += offset;
            }
            };
        expand(&Point::x);
        expand(&Point::y);
    }

    int Distance(Point p1, Point p2)
    {
        return std::abs(p2.x - p1.x) + std::abs(p2.y - p1.y);
    }

    size_t AllDistances(std::span<const Point> stars)
    {
        size_t sum = 0;
        for (size_t i1 = 0; i1 < stars.size() - 1; ++i1) {
            for (size_t i2 = i1 + 1; i2 < stars.size(); ++i2) {
                sum += static_cast<size_t>(Distance(stars[i1], stars[i2]));
            }
        }
        return sum;
    }

    size_t Solve(const std::filesystem::path& input, int factor)
    {
        auto stars = LoadStarPositions(input);
        Expand(stars, factor);
        return AllDistances(stars);
    }

    size_t Solve_1(const std::filesystem::path& input)
    {
        return Solve(input, 2);
    }

    size_t Solve_2(const std::filesystem::path& input)
    {
        return Solve(input, 1000000);
    }

    REGISTER_SOLUTION(2023, 11, 1, Solve_1);
    REGISTER_SOLUTION(2023, 11, 2, Solve_2);
}
