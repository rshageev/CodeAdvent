#include <scn/scan.h>
#include <Eigen/Dense>
#include "Runner.h"

namespace
{
    using Point64 = TPoint<int64>;
    constexpr auto IntMax = std::numeric_limits<int64>::max();

    struct Game {
        Point64 a;
        Point64 b;
        Point64 p;
    };

    auto LoadData(const std::filesystem::path& input, int64 offset = 0)
    {
        std::vector<Game> out;
        auto lines = ReadLines(input);
        for (size_t i = 0; i < lines.size(); i += 4) {
            Game g;
            if (auto res = scn::scan<int64, int64>(lines[i], "Button A: X+{}, Y+{}")) {
                std::tie(g.a.x, g.a.y) = res->values();
            }
            if (auto res = scn::scan<int64, int64>(lines[i + 1], "Button B: X+{}, Y+{}")) {
                std::tie(g.b.x, g.b.y) = res->values();
            }
            if (auto res = scn::scan<int64, int64>(lines[i + 2], "Prize: X={}, Y={}")) {
                std::tie(g.p.x, g.p.y) = res->values();
            }
            g.p.x += offset;
            g.p.y += offset;
            out.push_back(g);
        }
        return out;
    }

    int64 SolveGame_BruteForce(Game g)
    {
        int64 ap_max = std::min(g.p.x / g.a.x, g.p.y / g.a.y);

        int64 min_cost = IntMax;
        for (int64 ap = 0; ap < ap_max; ++ap)
        {
            Point64 prize = g.p - g.a * ap;
            int64 bp1 = prize.x / g.b.x;
            int64 bp2 = prize.y / g.b.y;
            if ((bp1 == bp2) && g.b * bp1 == prize) {
                min_cost = std::min(min_cost, ap * 3 + bp1);
            }
        }
        return (min_cost < IntMax) ? min_cost : 0;
    }

    int64 SolveGame_PenAndPaper(Game g)
    {
        /*
        |ax bx| * |pa| == |px|
        |ay by|   |pb|    |py|

        |INV| * |px| == |pa|
        |   |   |py|    |pb|

        | by -bx | * |px| == |pa| * det;
        | -ay ax |   |py|    |pb|
        */

        int64 det = g.a.x * g.b.y - g.b.x * g.a.y;
        if (det == 0) {
            return IntMax;
        }

        int64 pa = (g.b.y * g.p.x - g.b.x * g.p.y) / det;
        int64 pb = (g.a.x * g.p.y - g.a.y * g.p.x) / det;

        if (g.a * pa + g.b * pb == g.p) {
            return pa * 3 + pb;
        }
        return 0;
    }

    int64 SolveGame_Eigen(Game g)
    {
        auto M = Eigen::Matrix2d{
            { (double)g.a.x, (double)g.b.x },
            { (double)g.a.y, (double)g.b.y },
        };
        auto p = Eigen::Vector2d{ g.p.x, g.p.y };

        auto res = M.fullPivLu().solve(p);

        int64 pa = static_cast<int64>(std::round(res.x()));
        int64 pb = static_cast<int64>(std::round(res.y()));

        if (g.a * pa + g.b * pb == g.p) {
            return pa * 3 + pb;
        }
        return 0;
    }

    using SolveFunc = int64(*) (Game);

    int64 Solve(SolveFunc solve_game, int64 offset, const std::filesystem::path& input)
    {
        return Sum(LoadData(input, offset) | stdv::transform(solve_game));
    }

    REGISTER_SOLUTION(2024, 13, 1, (std::bind_front(Solve, SolveGame_PenAndPaper, 0)), "formula");
    REGISTER_SOLUTION(2024, 13, 1, (std::bind_front(Solve, SolveGame_Eigen, 0)), "eigen");
    REGISTER_SOLUTION(2024, 13, 1, (std::bind_front(Solve, SolveGame_BruteForce, 0)), "brute force");
    REGISTER_SOLUTION(2024, 13, 2, (std::bind_front(Solve, SolveGame_PenAndPaper, 10000000000000)), "formula");
    REGISTER_SOLUTION(2024, 13, 2, (std::bind_front(Solve, SolveGame_Eigen, 10000000000000)), "eigen");
}