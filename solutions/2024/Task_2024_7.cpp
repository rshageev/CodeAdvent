#include "Runner.h"

import std;
import utils;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<std::pair<uint64, std::vector<uint64>>> data;
        for (std::string_view line : ReadLines(input)) {
            auto& entry = data.emplace_back();
            entry.first = Read<uint64>(line);
            Skip(line, ": ");
            do {
                entry.second.push_back(Read<uint64>(line));
            } while (Skip(line, " "));
        }
        return data;
    }

    constexpr uint64 Concat(uint64 x, uint64 y)
    {
        uint64 n = y;
        while (n != 0) {
            n = n / 10;
            x *= 10;
        }
        return x + y;
    }

    template<int Part>
    bool CanBeSolved(uint64 expected_res, std::span<const uint64> values, uint64 acc_res)
    {
        if (values.empty()) {
            return acc_res == expected_res;
        }

        return CanBeSolved<Part>(expected_res, values.subspan(1), acc_res * values[0])
            || CanBeSolved<Part>(expected_res, values.subspan(1), acc_res + values[0])
            || ((Part == 2) && CanBeSolved<Part>(expected_res, values.subspan(1), Concat(acc_res, values[0])));
    }

    template<int Part>
    bool CanBeSolved(uint64 expected_res, std::span<const uint64> values)
    {
        return CanBeSolved<Part>(expected_res, values.subspan(1), values[0]);
    }

    template<int Part>
    uint64 Solve(const std::filesystem::path& input)
    {
        uint64 result = 0;
        for (const auto& [res, vs] : LoadData(input)) {
            if (CanBeSolved<Part>(res, vs)) {
                result += res;
            }
        }
        return result;
    }

    uint64 Solve_1(const std::filesystem::path& input)
    {
        return Solve<1>(input);
    }

    uint64 Solve_2(const std::filesystem::path& input)
    {
        return Solve<2>(input);
    }

    REGISTER_SOLUTION(2024, 7, 1, Solve_1);
    REGISTER_SOLUTION(2024, 7, 2, Solve_2);
}
