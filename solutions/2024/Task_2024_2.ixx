export module AoC_2024.Day2;

import std;
import utils;

export size_t Solve_1(const std::filesystem::path& input);
export size_t Solve_2(const std::filesystem::path& input);

module : private;

std::vector<int> ParseLine(std::string_view str)
{
    std::vector<int> res;
    do {
        res.push_back(Read<int>(str));
    } while (Skip(str, " "));
    return res;
}

auto LoadReports(const std::filesystem::path& input)
{
    return ReadLines(input)
        | stdv::transform(ParseLine)
        | stdr::to<std::vector>();
}

bool IsSafe(std::span<const int> levels)
{
    bool inc = false;
    bool dec = false;
    for (auto [v1, v2] : levels | stdv::pairwise)
    {
        if (v1 > v2) dec = true;
        if (v2 > v1) inc = true;
        int diff = std::abs(v2 - v1);
        if (diff < 1 || diff > 3) {
            return false;
        }
    }
    return inc != dec;
}

bool IsSafe2(const std::vector<int>& levels)
{
    if (IsSafe(levels))
        return true;

    for (size_t i = 0; i < levels.size(); ++i) {
        auto copy = levels;
        copy.erase(copy.begin() + i);
        if (IsSafe(copy))
            return true;
    }
    return false;
}

size_t Solve_1(const std::filesystem::path& input)
{
    return stdr::count_if(LoadReports(input), IsSafe);
}

size_t Solve_2(const std::filesystem::path& input)
{
    return stdr::count_if(LoadReports(input), IsSafe2);
}