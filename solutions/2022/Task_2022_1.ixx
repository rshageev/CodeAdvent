export module AoC_2022.Day1;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

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

    const auto itr = std::max_element(food.begin(), food.end());
    return *itr;
}

int Solve_2(const std::filesystem::path& input)
{
    auto food = ParseInput(input);

    stdr::sort(food, std::greater{});

    food.resize(3u);

    return std::accumulate(food.begin(), food.end(), 0);
}