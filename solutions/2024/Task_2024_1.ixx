module;
#include <scn/scan.h>

export module AoC_2024.Day1;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

auto ReadData(const std::filesystem::path& input)
{
    std::pair<std::vector<int>, std::vector<int>> data;
    for (auto line : ReadLines(input)) {
        auto [v1, v2] = scn::scan<int, int>(line, "{}   {}")->values();
        data.first.push_back(v1);
        data.second.push_back(v2);
    }
    return data;
}

int Solve_1(const std::filesystem::path& input)
{
    auto [list1, list2] = ReadData(input);

    stdr::sort(list1);
    stdr::sort(list2);

    int result = 0;
    for (auto [v1, v2] : stdv::zip(list1, list2)) {
        result += std::abs(v2 - v1);
    }

    return result;
}

int Solve_2(const std::filesystem::path& input)
{
    auto [list1, list2] = ReadData(input);

    int result = 0;
    for (int v1 : list1) {
        result += v1 * stdr::count(list2, v1);
    }
    return result;
}