#include "Runner.h"

import std;
import utils;

namespace
{
    auto Solve_1(const std::filesystem::path& input)
    {
        auto lines = ReadLines(input);

        std::string cl = lines[0];

        uint64 splits = 0;
        for (const auto& line : lines | stdv::drop(1)) {
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == '^' && (cl[i] == '|' || cl[i] == 'S')) {
                    cl[i - 1] = '|';
                    cl[i + 1] = '|';
                    cl[i] = '.';
                    ++splits;
                }
            }
        }
        return splits;
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        auto lines = ReadLines(input);

        auto tls = lines[0] | stdv::transform(equal<'S'>) | stdr::to<std::vector<uint64>>();

        for (const auto& line : lines | stdv::drop(1)) {
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == '^') {
                    tls[i - 1] += tls[i];
                    tls[i + 1] += tls[i];
                    tls[i] = 0;
                }
            }
        }
        return Sum(tls);
    }

    REGISTER_SOLUTION(2025, 7, 1, Solve_1);
    REGISTER_SOLUTION(2025, 7, 2, Solve_2);
}
