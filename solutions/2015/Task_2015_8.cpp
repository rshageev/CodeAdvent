#include "Runner.h"

import std;
import utils;

namespace
{
    size_t MemorySize(std::string_view str)
    {
        size_t size = 0;
        for (size_t i = 1; i < str.size() - 1; ++i) {
            if (str[i] == '\\') {
                i += (str[i + 1] == 'x') ? 3 : 1;
            }
            ++size;
        }
        return size;
    }

    size_t CodeSize(std::string_view str)
    {
        return str.size() + stdr::count(str, '"') + stdr::count(str, '\\') + 2;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        size_t res = 0;
        for (const auto line : ReadLines(input)) {
            res += (line.size() - MemorySize(line));
        }
        return res;
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        size_t res = 0;
        for (const auto line : ReadLines(input)) {
            res += (CodeSize(line) - line.size());
        }
        return res;
    }

    REGISTER_SOLUTION(2015, 8, 1, Solve_1);
    REGISTER_SOLUTION(2015, 8, 2, Solve_2);
}

