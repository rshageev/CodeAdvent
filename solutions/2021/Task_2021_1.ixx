export module AoC_2021.Day1;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

int CountIncreases(auto&& numbers)
{
    int counter = 0;
    int prev = numbers[0];
    for (int num : numbers | stdv::drop(1)) {
        if (num > prev) {
            ++counter;
        }
        prev = num;
    }
    return counter;
}

struct CountSum {
    int operator()(auto&& numbers) const {
        return std::accumulate(stdr::begin(numbers), stdr::end(numbers), 0);
    }
};

std::vector<int> ReadNumbers(const std::filesystem::path& input)
{
    return ReadLines(input)
        | stdv::filter(not_empty)
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
        | stdv::transform(CountSum{})
        | stdr::to<std::vector>();

    return CountIncreases(sums);
}