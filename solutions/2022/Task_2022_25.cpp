#include "Runner.h"

import std;
import utils;

namespace
{
    /* Decimal to SNAFU */

    constexpr int64 Spec2Dec(char ch)
    {
        switch (ch) {
        case '=': return -2;
        case '-': return -1;
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        }
        return 0;
    }

    constexpr int64 Spec2Dec(std::string_view str)
    {
        int64 value = 0;
        for (char ch : str) {
            value *= 5;
            value += Spec2Dec(ch);
        }
        return value;
    }


    /* SNAFU to decimal */

    constexpr std::pair<int64, int64> ExtractDigit(int64 num)
    {
        int64 quot = num / 5;
        int64 rem = num % 5;
        if (rem > 2) {
            rem -= 5;
            quot += 1;
        }
        return { quot, rem };
    }

    constexpr std::string Dec2Spec(int64 num)
    {
        constexpr char digits[] = { '=', '-', '0', '1', '2' };

        std::string str;
        std::pair<int64, int64> qr = { num, 0 };

        while (qr.first != 0) {
            qr = ExtractDigit(qr.first);
            str.push_back(digits[qr.second + 2]);
        }

        stdr::reverse(str);
        return str;
    }

    /* Tests */

    constexpr std::pair<int64, std::string_view> test_cases[] = {
        {1, "1"},
        {2, "2"},
        {3, "1="},
        {4, "1-"},
        {5, "10"},
        {6, "11"},
        {7, "12"},
        {8, "2="},
        {9, "2-"},
        {10, "20"},
        {15, "1=0"},
        {20, "1-0"},
        {37, "122"},
        {107, "1-12"},
        {2022, "1=11-2"},
        {4890, "2=-1=0"},
        {12345, "1-0---0"},
        {314159265, "1121-1110-1=0"},
    };

    static_assert(stdr::all_of(test_cases, [](auto p) { return Spec2Dec(p.second) == p.first; }));

    static_assert(stdr::all_of(test_cases, [](auto p) { return Dec2Spec(p.first) == p.second; }));

    /* Solve the task */

    std::string Solve_1(const std::filesystem::path& input)
    {
        int64 total = 0;
        for (const auto& line : ReadLines(input))
        {
            total += Spec2Dec(line);
        }
        return Dec2Spec(total);
    }

    REGISTER_SOLUTION(2022, 25, 1, Solve_1);
}