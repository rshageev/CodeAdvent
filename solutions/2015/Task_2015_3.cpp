#include "Runner.h"

import std;
import utils;

namespace
{
    size_t Solve_1(const std::filesystem::path& input)
    {
        const auto data = ReadText(input);

        Point current = { 0, 0 };
        std::set<Point> visited = { current };

        for (char ch : data) {
            current += DirToOffset(CharToDir(ch));
            visited.insert(current);
        }

        return visited.size();
    }

    size_t Solve_2(const std::filesystem::path& input)
    {
        const auto data = ReadText(input);

        Point current[2];
        size_t cnt_idx = 0;

        std::set<Point> visited = { {0, 0} };

        for (char ch : data) {
            current[cnt_idx] += DirToOffset(CharToDir(ch));
            visited.insert(current[cnt_idx]);
            cnt_idx = (cnt_idx + 1) % std::size(current);
        }

        return visited.size();
    }

    REGISTER_SOLUTION(2015, 3, 1, Solve_1);
    REGISTER_SOLUTION(2015, 3, 2, Solve_2);
}