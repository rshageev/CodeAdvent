module;
#include <scn/scan.h>

export module AoC_2024.Day3;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

/* ScnLib solution */

int Solve1_ScnLib(std::string_view str)
{
    int result = 0;
    while (!str.empty()) {
        if (auto sr = scn::scan<int, int>(str, "mul({},{})")) {
            auto [v1, v2] = sr->values();
            result += v1 * v2;
        }
        str.remove_prefix(1);
    }
    return result;
}

int Solve2_ScnLib(std::string_view str)
{
    bool enabled = true;
    int result = 0;
    while (!str.empty()) {
        if (str.starts_with("don't()")) {
            enabled = false;
        } else if (str.starts_with("do()")) {
            enabled = true;
        } if (enabled) {
            if (auto sr = scn::scan<int, int>(str, "mul({},{})")) {
                auto [v1, v2] = sr->values();
                result += v1 * v2;
            }
        }
        str.remove_prefix(1);
    }
    return result;
}

/* Regex solution */

int Solve1_Regex(const std::string& str)
{
    std::regex mul_regex(R"(mul\((\d+),(\d+)\))");
    std::sregex_iterator itr(str.begin(), str.end(), mul_regex), end;

    int result = 0;
    for (std::smatch match : stdr::subrange(itr, end)) {
        result += to_int(match[1].str()) * to_int(match[2].str());
    }
    return result;
}

int Solve2_Regex(const std::string& str)
{
    std::regex mul_regex(R"(mul\((\d+),(\d+)\)|don't\(\)|do\(\))");
    std::sregex_iterator itr(str.begin(), str.end(), mul_regex), end;

    int result = 0;
    bool enabled = true;
    for (std::smatch match : stdr::subrange(itr, end)) {
        if (match.str() == "don't()") {
            enabled = false;
        } else if (match.str() == "do()") {
            enabled = true;
        } else if (enabled) {
            result += to_int(match[1].str()) * to_int(match[2].str());
        }
    }
    return result;
}

/* Own parsing implementation */

int Solve1_MyParse(std::string_view str)
{
    int result = 0;
    while (!str.empty()) {
        int v1, v2;
        if (Parse{ str }.Skip("mul(").Read(v1).Skip(",").Read(v2).Skip(")").success) {
            result += v1 * v2;
        }
        str.remove_prefix(1);
    }
    return result;
}

int Solve2_MyParse(std::string_view str)
{
    int result = 0;
    bool enabled = true;
    while (!str.empty()) {
        if (str.starts_with("don't()")) {
            enabled = false;
        } else if (str.starts_with("do()")) {
            enabled = true;
        } else if (enabled) {
            int v1, v2;
            if (Parse{ str }.Skip("mul(").Read(v1).Skip(",").Read(v2).Skip(")").success) {
                result += v1 * v2;
            }
        }
        str.remove_prefix(1);
    }
    return result;
}

/**************************************************************/

int Solve_1(const std::filesystem::path& input)
{
    return Solve1_MyParse(ReadText(input));
}

int Solve_2(const std::filesystem::path& input)
{
    return Solve2_MyParse(ReadText(input));
}