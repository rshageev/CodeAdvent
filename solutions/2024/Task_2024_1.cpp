#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
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
            result += v1 * static_cast<int>(stdr::count(list2, v1));
        }
        return result;
    }

    REGISTER_SOLUTION(2024, 1, 1, Solve_1);
    REGISTER_SOLUTION(2024, 1, 2, Solve_2);
}