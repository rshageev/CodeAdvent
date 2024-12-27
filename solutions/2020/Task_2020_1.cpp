#include "Runner.h"

import std;
import utils;

namespace
{
    auto ReadNumbers(const std::filesystem::path& input) {
        return ReadLines(input)
            | stdv::transform(to_int)
            | stdr::to<std::vector>();
    }

    int Solve_1(const std::filesystem::path& input)
    {
        const auto data = ReadNumbers(input);

        for (auto [i1,v1] : data | stdv::enumerate) {
            for (auto [i2, v2] : data | stdv::enumerate) {
                if (i1 != i2 && v1 + v2 == 2020) {
                    return v1 * v2;
                }
            }
        }
        return 0;
    }

    int Solve_2(const std::filesystem::path& input)
    {
        const auto data = ReadNumbers(input);

        for (auto [i1, v1] : data | stdv::enumerate) {
            for (auto [i2, v2] : data | stdv::enumerate) {
                for (auto [i3, v3] : data | stdv::enumerate) {
                    if (i1 != i2 && i1 != i3 && i2 != i3 && v1 + v2 + v3 == 2020)
                    {
                        return v1 * v2 * v3;
                    }
                }
            }
        }
        return 0;
    }

    REGISTER_SOLUTION(2020, 1, 1, Solve_1);
    REGISTER_SOLUTION(2020, 1, 2, Solve_2);
}