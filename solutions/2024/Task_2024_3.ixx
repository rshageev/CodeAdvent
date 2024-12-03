module;
#include <scn/scan.h>

export module AoC_2024.Day3;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

int Solve_1(const std::filesystem::path& input)
{
    int result = 0;
    const auto data = ReadText(input);
    std::string_view str(data);
    while (!str.empty()) {
        if (auto sr = scn::scan<int,int>(str, "mul({},{})")) {
            auto [v1, v2] = sr->values();
            result += v1 * v2;
        }
        str.remove_prefix(1);
    }
    return result;
}

int Solve_2(const std::filesystem::path& input)
{
    bool enabled = true;
    int result = 0;
    const auto data = ReadText(input);
    std::string_view str(data);
    while (!str.empty()) {   
        if (str.starts_with("don't()")) {
            enabled = false;
        } else if (str.starts_with("do()")) {
            enabled = true;
        }
        if (enabled) {
            if (auto sr = scn::scan<int, int>(str, "mul({},{})")) {
                auto [v1, v2] = sr->values();
                result += v1 * v2;
            }
        }
        str.remove_prefix(1);
    }
    return result;
}