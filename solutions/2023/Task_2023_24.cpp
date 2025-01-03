#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    using Point64 = TPoint<int64>;
    using Point64d = TPoint<double>;

    struct Stone
    {
        int64 px, py, pz;
        int64 vx, vy, vz;
    };

    Stone ParseLine(std::string_view str)
    {
        if (auto res = scn::scan<int64, int64, int64, int64, int64, int64>(str, "{}, {}, {} @ {}, {}, {}")) {
            auto [x, y, z, vx, vy, vz] = res->values();
            return Stone{ x, y, z, vx, vy, vz };
        }
        return Stone{};
    }

    auto LoadInput(const std::filesystem::path& input)
    {
        return ReadLines(input) | stdv::transform(ParseLine) | stdr::to<std::vector>();
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

    int64 CountIntersections(std::span<const Stone> stones, int64 area_min, int64 area_max)
    {
        const auto offset = area_min;
        area_min = 0;
        area_max -= offset;

        int64 count = 0;
        for (size_t i = 1; i < stones.size(); ++i)
        {
            for (size_t j = 0; j < i; ++j)
            {
                Point64 p1 = { stones[i].px - offset, stones[i].py - offset };
                Point64 v1 = { stones[i].vx, stones[i].vy };
                Point64 p2 = { stones[j].px - offset, stones[j].py - offset };
                Point64 v2 = { stones[j].vx, stones[j].vy };

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

    std::tuple<int64, int64, int64> NormalizeStoneCoords(std::span<Stone> stones)
    {
        const auto& [min_xs, max_xs] = stdr::minmax(stones, std::less{}, &Stone::px);
        const auto& [min_ys, max_ys] = stdr::minmax(stones, std::less{}, &Stone::py);
        const auto& [min_zs, max_zs] = stdr::minmax(stones, std::less{}, &Stone::pz);

        int64 offset_x = std::midpoint(min_xs.px, max_xs.px);
        int64 offset_y = std::midpoint(min_ys.py, max_ys.py);
        int64 offset_z = std::midpoint(min_zs.pz, max_zs.pz);

        for (auto& s : stones) {
            s.px -= offset_x;
            s.py -= offset_y;
            s.pz -= offset_z;
        }
        return { offset_x, offset_y, offset_z };
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        const bool is_test = (input.filename() != "input.txt");
        const int64 MinLimit = is_test ? 7 : 200000000000000;
        const int64 MaxLimit = is_test ? 27 : 400000000000000;

        const auto stones = LoadInput(input);

        return CountIntersections(stones, MinLimit, MaxLimit);
    }

    using EqSolver = std::vector<double>(*)(const std::vector<std::vector<double>>&);

    int64 Solve_2(EqSolver solveSystem, const std::filesystem::path& input)
    {
        auto stones = LoadInput(input);

        // Move stone coordinates closer to 0 to help with precision
        auto [off_x, off_y, off_z] = NormalizeStoneCoords(stones | stdv::take(3));

        std::vector<std::vector<double>> equations;

        auto add_equations = [&](const Stone& s0, const Stone& s1) {
            equations.push_back({ // XY
                // coeff
                double(s0.vy - s1.vy),
                double(s1.vx - s0.vx),
                0.0,
                double(s1.py - s0.py),
                double(s0.px - s1.px),
                0.0,
                // result
                double(s1.py * s1.vx - s1.px * s1.vy - s0.py * s0.vx + s0.px * s0.vy)
            }); 
            equations.push_back({ // XZ
                // coeff
                double(s0.vz - s1.vz),
                0.0,
                double(s1.vx - s0.vx),
                double(s1.pz - s0.pz),
                0.0,
                double(s0.px - s1.px),
                // result
                double(s1.pz * s1.vx - s1.px * s1.vz - s0.pz * s0.vx + s0.px * s0.vz)
            });
            equations.push_back({ // YZ
                // coeff
                0.0,
                double(s0.vz - s1.vz),
                double(s1.vy - s0.vy),
                0.0,
                double(s1.pz - s0.pz),
                double(s0.py - s1.py),
                // result
                double(s1.pz * s1.vy - s1.py * s1.vz - s0.pz * s0.vy + s0.py * s0.vz)
            });
        };

        add_equations(stones[0], stones[1]);
        add_equations(stones[0], stones[2]);

        auto sln = solveSystem(equations);

        int64 px = static_cast<int64>(std::round(sln[0]));
        int64 py = static_cast<int64>(std::round(sln[1]));
        int64 pz = static_cast<int64>(std::round(sln[2]));

        return px + py + pz + off_x + off_y + off_z;
    }

    REGISTER_SOLUTION(2023, 24, 1, Solve_1);
    REGISTER_SOLUTION(2023, 24, 2, std::bind_front(Solve_2, SolveSystemEigen), "eigen");
    REGISTER_SOLUTION(2023, 24, 2, std::bind_front(Solve_2, SolveSystemGauss), "gauss");
}
