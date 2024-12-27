#include "Runner.h"

import std;
import utils;

namespace
{
    std::vector<int> ParseInput(const std::filesystem::path& input)
    {
        std::vector<int> elf_food;
        for (auto elf_lines : ReadLines(input) | stdv::split(""))
        {
            int food = 0;
            for (auto food_item : elf_lines | stdv::transform(to_int)) {
                food += food_item;
            }
            elf_food.push_back(food);
        }
        return elf_food;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        const auto food = ParseInput(input);

        return *std::max_element(food.begin(), food.end());
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