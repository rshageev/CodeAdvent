#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    std::string Step(std::string_view str)
    {
        char digit = str[0];
        int count = 1;

        std::string res;
        res.reserve(str.size() * 3 / 2);
        for (char ch : str.substr(1)) {
            if (ch == digit) {
                ++count;
            } else {
                std::format_to(std::back_inserter(res), "{}{}", count, digit);
                digit = ch;
                count = 1;
            }
        }
        std::format_to(std::back_inserter(res), "{}{}", count, digit);
        return res;
    }

    auto Solve(int iters, const std::filesystem::path& input)
    {
        std::string str = ReadText(input);
        for (int i = 0; i < iters; ++i) {
            str = Step(str);
        }
        return str.size();
    }

    REGISTER_SOLUTION(2015, 10, 1, std::bind_front(Solve, 40));
    REGISTER_SOLUTION(2015, 10, 2, std::bind_front(Solve, 50));
}
