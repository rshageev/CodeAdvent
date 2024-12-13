module;
#include <scn/scan.h>

export module AoC_2024.Day13;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

using Point64 = TPoint<int64>;
constexpr auto IntMax = std::numeric_limits<int64>::max();

struct Game {
    Point64 a;
    Point64 b;
    Point64 p;
};

auto LoadData(const std::filesystem::path& input)
{
    std::vector<Game> out;
    auto lines = ReadLines(input);
    for (size_t i = 0; i < lines.size(); i += 4) {
        Game g;
        if (auto res = scn::scan<int64, int64>(lines[i], "Button A: X+{}, Y+{}")) {
            std::tie(g.a.x, g.a.y) = res->values();
        }
        if (auto res = scn::scan<int64, int64>(lines[i+1], "Button B: X+{}, Y+{}")) {
            std::tie(g.b.x, g.b.y) = res->values();
        }
        if (auto res = scn::scan<int64, int64>(lines[i+2], "Prize: X={}, Y={}")) {
            std::tie(g.p.x, g.p.y) = res->values();
        }
        out.push_back(g);
    }
    return out;
}

//int64 SolveGame(Game g)
//{
//    int64 ap_max = std::min(g.p.x / g.a.x, g.p.y / g.a.y);
//
//    int64 min_cost = IntMax;
//    for (int64 ap = 0; ap < ap_max; ++ap)
//    {
//        Point64 prize = g.p - g.a * ap;
//        int64 bp1 = prize.x / g.b.x;
//        int64 bp2 = prize.y / g.b.y;
//        if ((bp1 == bp2) && g.b * bp1 == prize) {
//            min_cost = std::min(min_cost, ap * 3 + bp1);
//        }
//    }
//    return min_cost;
//}

int64 SolveGame(Game g)
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
    return IntMax;
}

int64 Solve_1(const std::filesystem::path& input)
{
    auto games = LoadData(input);
    int64 total = 0;
    for (auto game : games) { 
        if (int64 cost = SolveGame(game); cost < IntMax) {
            total += cost;
        }
    }
    return total;
}

int64 Solve_2(const std::filesystem::path& input)
{
    auto games = LoadData(input);
    int64 total = 0;
    for (auto game : games) {
        game.p.x += 10000000000000;
        game.p.y += 10000000000000;    
        if (int64 cost = SolveGame(game); cost < IntMax) {
            total += cost;
        }
    }
    return total;
}