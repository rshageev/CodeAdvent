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

        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size(); ++j) {
                if (i != j && data[i] + data[j] == 2020) {
                    return data[i] * data[j];
                }
            }
        }
        return 0;
    }

    int Solve_2(const std::filesystem::path& input)
    {
        const auto data = ReadNumbers(input);

        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size(); ++j) {
                for (size_t k = 0; k < data.size(); ++k) {
                    if (i != j && i != k && j != k && data[i] + data[j] + data[k] == 2020)
                    {
                        return data[i] * data[j] * data[k];
                    }
                }
            }
        }
        return 0;
    }

    REGISTER_SOLUTION(2020, 1, 1, Solve_1);
    REGISTER_SOLUTION(2020, 1, 2, Solve_2);
}