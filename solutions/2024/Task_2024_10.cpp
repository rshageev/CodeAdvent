#include "Runner.h"

import std;
import utils;

namespace
{
    int FindTrailScore(const Array2D<int>& map, Point start)
    {
        std::queue<Point> q;
        q.push(start);

        std::set<Point> ends;

        while (!q.empty()) {
            Point pos = q.front();
            q.pop();
            int h = map[pos] + 1;

            for (auto [dir, off] : Neighbours4) {
                Point np = pos + off;
                if (map.Contains(np) && map[np] == h) {
                    if (h == 9)
                        ends.insert(np);
                    else
                        q.push(np);
                }
            }
        }
        return static_cast<int>(ends.size());
    }

    int FindTrailRating(const Array2D<int>& map, Point start)
    {
        std::queue<Point> q;
        q.push(start);

        int rating = 0;
        while (!q.empty()) {
            Point pos = q.front();
            q.pop();

            int h = map[pos] + 1;

            for (auto [dir, off] : Neighbours4) {
                Point np = pos + off;
                if (map.Contains(np) && map[np] == h) {
                    if (h == 9)
                        ++rating;
                    else
                        q.push(np);
                }
            }
        }
        return rating;
    }

    int Solve(const std::filesystem::path& input, auto calcScore)
    {
        auto map = ReadArray2D(input, to_int);

        int score = 0;
        for (Point start : to_cell_coords(map)) {
            if (map[start] == 0) {
                score += calcScore(map, start);
            }
        }
        return score;
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        return Solve(input, FindTrailScore);
    }

    int64 Solve_2(const std::filesystem::path& input)
    {
        return Solve(input, FindTrailRating);
    }

    REGISTER_SOLUTION(2024, 10, 1, Solve_1);
    REGISTER_SOLUTION(2024, 10, 2, Solve_2);
}