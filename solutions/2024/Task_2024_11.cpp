#include "Runner.h"

import std;
import utils;

namespace
{

    std::unordered_map<uint64, uint64> ReadData(const std::filesystem::path& input)
    {
        const auto data = ReadText(input);
        std::string_view str = data;
        std::unordered_map<uint64, uint64> stone_counts;
        do {
            stone_counts[Read<uint64>(str)]++;
        } while (Skip(str, " "));
        return stone_counts;
    }

    std::vector<uint64> Blink(uint64 s)
    {
        if (s == 0) {
            return { 1 };
        }

        std::string str = std::to_string(s);
        if (str.size() % 2 == 0) {
            uint64 v1 = from_chars_to<uint64>{}(str.substr(0, str.size() / 2));
            uint64 v2 = from_chars_to<uint64>{}(str.substr(str.size() / 2));
            return { v1, v2 };
        }

        return { s * 2024 };
    }

    std::unordered_map<uint64, uint64> Blink(const std::unordered_map<uint64, uint64>& counts)
    {
        std::unordered_map<uint64, uint64> out;
        for (auto [s, cnt] : counts) {
            for (auto ns : Blink(s)) {
                out[ns] += cnt;
            }
        }
        return out;
    }

    uint64 Solve(std::unordered_map<uint64, uint64> stone_counts, int blinks)
    {
        for (int i = 0; i < blinks; ++i) {
            stone_counts = Blink(stone_counts);
        }

        return Sum(stone_counts | stdv::values);
    }

    uint64 Solve_1(const std::filesystem::path& input)
    {
        return Solve(ReadData(input), 25);
    }

    uint64 Solve_2(const std::filesystem::path& input)
    {
        return Solve(ReadData(input), 75);
    }

    REGISTER_SOLUTION(2024, 11, 1, Solve_1);
    REGISTER_SOLUTION(2024, 11, 2, Solve_2);
}