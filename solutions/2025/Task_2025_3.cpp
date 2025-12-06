#include "Runner.h"

import std;
import utils;

namespace
{
    uint64 MaxJoltageBF(std::string_view str)
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

    template<size_t N>
    uint64 MaxJoltage(std::string_view str)
    {
        uint64 sum = 0;
        for (size_t i = N; i > 0; --i) {
            auto ss = str.substr(0, str.size() - i + 1);
            auto max_itr = stdr::max_element(ss);
            str.remove_prefix(max_itr - ss.begin() + 1);
            sum = sum * 10 + to_uint64(*max_itr);
        }
        return sum;
    }

    using MaxJoltageFunc = uint64(*) (std::string_view);
    auto Solve(MaxJoltageFunc max_joltage, const std::filesystem::path& input)
    {
        return Sum(ReadLines(input) | stdv::transform(max_joltage));
    }

    REGISTER_SOLUTION(2025, 3, 1, std::bind_front(Solve, MaxJoltageBF), "brute force");
    REGISTER_SOLUTION(2025, 3, 1, std::bind_front(Solve, MaxJoltage<2>));
    REGISTER_SOLUTION(2025, 3, 2, std::bind_front(Solve, MaxJoltage<12>));
}
