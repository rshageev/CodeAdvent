#include "Runner.h"

import std;
import utils;

namespace
{
    void DoStep(const Array2D<char>& map, const std::unordered_set<Point>& current, std::unordered_set<Point>& next)
    {
        for (Point pt : current)
        {
            for (auto [dir, off] : Neighbours4)
            {
                Point pos = pt + off;
                if (map.Contains(pos) && map[pos] != '#')
                {
                    next.insert(pos);
                }
            }
        }
    }

    size_t Solve_1(const std::filesystem::path& input)
    {
        const auto map = ReadArray2D(input);
        const Point start = FindInArray2D(map, 'S');

        std::unordered_set<Point> current = { start };
        std::unordered_set<Point> next;

        for (int i = 0; i < 64; ++i)
        {
            next.clear();
            DoStep(map, current, next);
            current = std::move(next);
        }

        return current.size();
    }



    void DoStep2(const Array2D<char>& map, const std::unordered_set<Point>& current, std::unordered_set<Point>& next)
    {
        for (Point pt : current)
        {
            for (auto [dir, off] : Neighbours4)
            {
                Point npos = pt + off;
                Point wpos = WrapPoint(npos, map.Area());
                if (map[wpos] != '#')
                {
                    next.insert(npos);
                }
            }
        }
    }

    size_t Solve_2(const std::filesystem::path& input)
    {
        constexpr int Steps = 26501365;

        const auto map = ReadArray2D(input);
        const Point start = FindInArray2D(map, 'S');

        std::unordered_set<Point> current = { start };
        std::unordered_set<Point> next;

        const auto period = map.Width();

        std::vector<int> deltas1(period, 0);
        std::vector<int> deltas2(period, 0);

        for (int i = 0; i < period; ++i)
        {
            next.clear();
            DoStep2(map, current, next);
            deltas1[i] = static_cast<int>(std::ssize(next) - std::ssize(current));
            current = std::move(next);
        }
        for (int i = 0; i < period; ++i)
        {
            next.clear();
            DoStep2(map, current, next);
            deltas2[i] = static_cast<int>(std::ssize(next) - std::ssize(current));
            current = std::move(next);
        }

        std::vector<int> dd(period, 0);
        for (int i = 0; i < period; ++i) {
            dd[i] = deltas2[i] - deltas1[i];
        }

        size_t sum = 1;
        for (int i = 0; i < Steps; ++i)
        {
            int substep = i % 131;
            int ddstep = i / 131;
            int delta = deltas1[substep] + dd[substep] * ddstep;
            sum += (size_t)delta;
        }

        return sum;
    }

    REGISTER_SOLUTION(2023, 21, 1, Solve_1);
    REGISTER_SOLUTION(2023, 21, 2, Solve_2);
}