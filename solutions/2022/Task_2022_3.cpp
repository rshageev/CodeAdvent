#include "Runner.h"

import std;
import utils;

namespace
{
    int Priority(char item)
    {
        if (item >= 'a' && item <= 'z') return item - 'a' + 1;
        if (item >= 'A' && item <= 'Z') return item - 'A' + 27;
        return 0;
    }

    std::bitset<52> ItemsSet(std::string_view str)
    {
        std::bitset<52> result;
        for (char ch : str) {
            result.set(Priority(ch) - 1, true);
        }
        return result;
    }

    char CommonItemPriority(const auto& strings)
    {
        std::bitset<52> item_set(0xFFFF'FFFF'FFFF'FFFF);
        for (std::string_view str : strings) {
            item_set &= ItemsSet(str);
        }
        return std::countr_zero(item_set.to_ullong()) + 1;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        int res = 0;
        for (std::string_view line : ReadLines(input))
        {
            std::string_view bags[2] = {
                line.substr(0, line.size() / 2),
                line.substr(line.size() / 2)
            };
            res += CommonItemPriority(bags);
        }
        return res;
    }

    int Solve_2(const std::filesystem::path& input)
    {
        int res = 0;
        for (auto&& bags : ReadLines(input) | stdv::chunk(3)) {
            res += CommonItemPriority(bags);
        }
        return res;
    }

    REGISTER_SOLUTION(2022, 3, 1, Solve_1);
    REGISTER_SOLUTION(2022, 3, 2, Solve_2);
}

