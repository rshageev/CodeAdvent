#include "Runner.h"

import std;
import utils;

namespace
{
    uint64 MaxJoltage(std::string_view str)
    {
        uint64 max = 0;
        for (size_t i = 0; i < str.size() - 1; ++i) {
            for (size_t j = i + 1; j < str.size(); ++j) {
                uint64 joltage = to_uint64(str[i]) * 10 + to_uint64(str[j]);
                if (joltage > max) {
                    max = joltage;
                }
            }
        }
        return max;
    }

    std::pair<uint64, size_t> MaxDigit(std::string_view str) {
        uint64 maxv = 0;
        size_t maxp = 0;
        size_t pos = 0;
        for (char ch : str) {
            uint64 digit = to_uint64(ch);
            if (digit > maxv) {
                maxv = digit;
                maxp = pos;
            }
            ++pos;
        }
        return std::make_pair(maxv, maxp);
    }

    uint64 MaxJoltage12(std::string_view str)
    {
        uint64 pow10 = 100'000'000'000;
        uint64 sum = 0;
        size_t pos = 0;
        for (size_t i = 12; i > 0; --i) {
            size_t rem = i - 1;
            std::string_view ss = str.substr(pos, str.size() - rem - pos);
            auto [v, p] = MaxDigit(ss);
            pos += (p + 1);
            sum += v * pow10;
            pow10 /= 10;
        }
        return sum;
    }

    using MaxJoltageFunc = uint64(*) (std::string_view);
    auto Solve(MaxJoltageFunc max_joltage, const std::filesystem::path& input)
    {
        uint64 sum = 0;
        for (const auto& line : ReadLines(input)) {
            sum += max_joltage(line);
        }
        return sum;
    }

    REGISTER_SOLUTION(2025, 3, 1, std::bind_front(Solve, MaxJoltage));
    REGISTER_SOLUTION(2025, 3, 2, std::bind_front(Solve, MaxJoltage12));
}
