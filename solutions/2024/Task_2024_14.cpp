#include <scn/scan.h>
#include "Runner.h"

namespace
{
    struct Robot
    {
        Point p;
        Point v;
    };

    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<Robot> robots;
        for (const auto& line : ReadLines(input))
        {
            if (auto res = scn::scan<int, int, int, int>(line, "p={},{} v={},{}")) {
                auto [x, y, dx, dy] = res->values();
                robots.push_back({ {x,y}, {dx,dy} });
            }
        }
        return robots;
    }

    std::array<int, 4> QuadDistr(std::span<const Robot> robots, const Rect& area)
    {
        std::array<int, 4> q = { 0, 0, 0, 0 };

        const int mx = area.w / 2;
        const int my = area.h / 2;

        for (auto r : robots) {
            if (r.p.x < mx) {
                if (r.p.y < my) {
                    q[0]++;
                }
                else if (r.p.y > my) {
                    q[1]++;
                }
            }
            else if (r.p.x > mx) {
                if (r.p.y < my) {
                    q[2]++;
                }
                else if (r.p.y > my) {
                    q[3]++;
                }
            }
        }

        return q;
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        auto robots = LoadData(input);

        const Rect area = (robots.size() > 15) ? Rect{ 0, 0, 101, 103 } : Rect{ 0, 0, 11, 7 };

        for (int i = 0; i < 100; ++i) {
            for (auto& r : robots) {
                r.p = WrapPoint(r.p + r.v, area);
            }
        }

        auto quads = QuadDistr(robots, area);

        return quads[0] * quads[1] * quads[2] * quads[3];
    }

    size_t Symmetrical(const Array2D<int>& map)
    {
        const auto w = map.Width();

        size_t sym = 0;
        for (Point pos : to_cell_coords(map))
        {
            if (map[pos] > 0) {
                Point sp = { w - pos.x - 1, pos.y };
                if (map[sp] > 0) ++sym;
            }
        }
        return sym;
    }

    int64 Solve_2(const std::filesystem::path& input)
    {
        auto robots = LoadData(input);

        const Rect area = (robots.size() > 15) ? Rect{ 0, 0, 101, 103 } : Rect{ 0, 0, 11, 7 };

        Array2D<int> map(area, 0);

        for (int i = 0; i <= 10000; ++i) {
            for (auto& r : robots) {
                r.p = WrapPoint(r.p + r.v, area);
            }

            std::fill(map.begin(), map.end(), 0);
            for (auto r : robots) {
                map[r.p]++;
            }

            if (Symmetrical(map) > robots.size() / 6) {
                std::cout << std::format("Step: {}\n", i);
                PrintArray2D(map, [](int c) -> char { return (c > 0) ? '#' : '.'; });
            }
        }

        return 0;
    }

    REGISTER_SOLUTION(2024, 14, 1, Solve_1);
    //REGISTER_SOLUTION(2024, 14, 2, Solve_2);
}
