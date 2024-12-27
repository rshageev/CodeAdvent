#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
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

        int result = 0;
        for (std::smatch match : RegexSearch(str, mul_regex)) {
            result += to_int(match[1].str()) * to_int(match[2].str());
        }
        return result;
    }

    int Solve2_Regex(const std::string& str)
    {
        std::regex mul_regex(R"(mul\((\d+),(\d+)\)|don't\(\)|do\(\))");

        int result = 0;
        bool enabled = true;
        for (std::smatch match : RegexSearch(str, mul_regex)) {
            if (match.str() == "don't()") {
                enabled = false;
            }
            else if (match.str() == "do()") {
                enabled = true;
            }
            else if (enabled) {
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
            }
            else if (str.starts_with("do()")) {
                enabled = true;
            }
            else if (enabled) {
                int v1, v2;
                if (Parse{ str }.Skip("mul(").Read(v1).Skip(",").Read(v2).Skip(")").success) {
                    result += v1 * v2;
                }
            }
            str.remove_prefix(1);
        }
        return result;
    }

    REGISTER_SOLUTION(2024, 3, 1, [](auto& input) { return Solve1_MyParse(ReadText(input)); }, "myparse");
    REGISTER_SOLUTION(2024, 3, 1, [](auto& input) { return Solve1_ScnLib(ReadText(input)); }, "scnlib");
    REGISTER_SOLUTION(2024, 3, 1, [](auto& input) { return Solve1_Regex(ReadText(input)); }, "regex");

    REGISTER_SOLUTION(2024, 3, 2, [](auto& input) { return Solve2_MyParse(ReadText(input)); }, "myparse");
    REGISTER_SOLUTION(2024, 3, 2, [](auto& input) { return Solve2_ScnLib(ReadText(input)); }, "scnlib");
    REGISTER_SOLUTION(2024, 3, 2, [](auto& input) { return Solve2_Regex(ReadText(input)); }, "regex");
}
