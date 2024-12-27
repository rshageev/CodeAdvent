#include "Runner.h"

import std;
import utils;

namespace
{
    int CountIncreases(auto&& numbers)
    {
        int counter = 0;
        for (auto [prev, cnt] : numbers | stdv::pairwise) {
            if (cnt > prev) {
                ++counter;
            }
        }
        return counter;
    }

    std::vector<int> ReadNumbers(const std::filesystem::path& input)
    {
        return ReadLines(input)
            | stdv::transform(to_int)
            | stdr::to<std::vector>();
    }

    int Solve_1(const std::filesystem::path& input)
    {
        const auto numbers = ReadNumbers(input);

        return CountIncreases(numbers);
    }

    int Solve_2(const std::filesystem::path& input)
    {
        const auto numbers = ReadNumbers(input);

        const auto sums = numbers
            | stdv::slide(3)
            | stdv::transform(Sum)
            | stdr::to<std::vector>();

        return CountIncreases(sums);
    }

    REGISTER_SOLUTION(2021, 1, 1, Solve_1);
    REGISTER_SOLUTION(2021, 1, 2, Solve_2);
}