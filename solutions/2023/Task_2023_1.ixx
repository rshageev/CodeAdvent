export module AoC_2023.Day1;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

int ReadNumber(std::string_view str)
{
    constexpr const char* digits = "0123456789";

    auto p1 = str.find_first_of(digits);
    auto p2 = str.find_last_of(digits);
    return (str[p1] - '0') * 10 + (str[p2] - '0');
}

int Solve_1(const std::filesystem::path& input)
{
    int sum = 0;
    for (auto line : ReadLines(input))
    {
        sum += ReadNumber(line);
    }
    return sum;
}

struct DigitDesc
{
    int value;
    std::array<std::string_view, 2> names;
};

constexpr std::array<DigitDesc, 10> Digits = {{
    { 0, { "0", "zero "} },
    { 1, { "1", "one" } },
    { 2, { "2", "two" } },
    { 3, { "3", "three"} },
    { 4, { "4", "four" } },
    { 5, { "5", "five" } },
    { 6, { "6", "six" } },
    { 7, { "7", "seven" } },
    { 8, { "8", "eight" } },
    { 9, { "9", "nine" } },
}};

int FindDigit(std::string_view str, auto find, auto cmp)
{
    int result = 0;
    size_t best_pos = std::string_view::npos;

    for (const auto& [digit, names] : Digits) {
        for (auto name : names) {
            const auto pos = find(str, name);
            if (cmp(pos, best_pos)) {
                best_pos = pos;
                result = digit;
            }
        }
    }

    return result;
}

int FindLeftDigit(std::string_view str)
{
    return FindDigit(str,
        [](std::string_view str, std::string_view substr) { return str.find(substr); },
        [](size_t p1, size_t p2) { return p1 < p2; });
}

int FindRightDigit(std::string_view str)
{
    return FindDigit(str,
        [](std::string_view str, std::string_view substr) { return str.rfind(substr); },
        [](size_t p1, size_t p2) {
            return p1 != std::string_view::npos && (p2 == std::string_view::npos || p1 > p2);
        });
}

int Solve_2(const std::filesystem::path& input)
{
    int sum = 0;
    for (auto line : ReadLines(input))
    {
        const int d1 = FindLeftDigit(line);
        const int d2 = FindRightDigit(line);
        const int value = d1 * 10 + d2;
        sum += value;
    }
    return sum;
}