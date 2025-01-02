#include "Runner.h"

import std;
import utils;

namespace
{
    std::vector<int> ParseInput(const std::filesystem::path& input)
    {
        std::vector<int> elf_food;
        for (auto elf_lines : ReadLineBlocks(input))
        {
            elf_food.push_back(Sum(elf_lines | stdv::transform(to_int)));
        }
        return elf_food;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        return stdr::max(ParseInput(input));
    }

    int Solve_2(const std::filesystem::path& input)
    {
        auto food = ParseInput(input);

        stdr::sort(food, std::greater{});

        return Sum(food | stdv::take(3));
    }

    REGISTER_SOLUTION(2022, 1, 1, Solve_1);
    REGISTER_SOLUTION(2022, 1, 2, Solve_2);
}