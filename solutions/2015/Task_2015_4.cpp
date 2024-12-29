#include "Runner.h"

import std;
import utils;
import md5;

namespace
{
    uint32 Solve_1(const std::filesystem::path& input)
    {
        const auto key = ReadText(input);

        for (uint32 i = 0; i < std::numeric_limits<uint32>::max(); ++i)
        {
            std::string str = std::format("{}{}", key, i);
            std::string hash = CalcMD5(str);
            if (hash.starts_with("00000")) {
                return i;
            }
        }
        return 0;
    }

    uint32 Solve_2(const std::filesystem::path& input)
    {
        const auto key = ReadText(input);

        for (uint32 i = 0; i < std::numeric_limits<uint32>::max(); ++i)
        {
            std::string str = std::format("{}{}", key, i);
            std::string hash = CalcMD5(str);
            if (hash.starts_with("000000")) {
                return i;
            }
        }
        return 0;
    }

    REGISTER_SOLUTION(2015, 4, 1, Solve_1);
    REGISTER_SOLUTION(2015, 4, 2, Solve_2);
}