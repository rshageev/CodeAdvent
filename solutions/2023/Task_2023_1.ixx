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
    for (auto line : ReadLines(input) | stdv::filter(not_empty))
    {
        sum += ReadNumber(line);
    }
    return sum;
}

struct Digit {
    int value;
    char ch;
    std::string_view name;
};

constexpr Digit digit_names[] = {
    { 0, '0', "zero"},
    { 1, '1', "one"},
    { 2, '2', "two"},
    { 3, '3', "three"},
    { 4, '4', "four"},
    { 5, '5', "five"},
    { 6, '6', "six"},
    { 7, '7', "seven"},
    { 8, '8', "eight"},
    { 9, '9', "nine"},
};

size_t FindDigitLeft(std::string_view str, const Digit& digit)
{
    const auto p1 = str.find(digit.ch);
    const auto p2 = str.find(digit.name);

    if (p1 == std::string_view::npos) {
        return p2;
    } else if (p2 == std::string_view::npos) {
        return p1;
    }
    return std::min(p1, p2);
}

size_t FindDigitRight(std::string_view str, const Digit& digit)
{
    const auto p1 = str.rfind(digit.ch);
    const auto p2 = str.rfind(digit.name);

    if (p1 == std::string_view::npos) {
        return p2;
    } else if (p2 == std::string_view::npos) {
        return p1;
    }
    return std::max(p1, p2);
}

int FindDigitLeft(std::string_view str)
{
    size_t lpos = str.size();
    int value = 0;
    for (const auto& digit : digit_names) {
        const size_t p = FindDigitLeft(str, digit);
        if (p != std::string_view::npos && p <= lpos) {
            lpos = p;
            value = digit.value;
        }
    }
    return value;
}

int FindDigitRight(std::string_view str)
{
    size_t lpos = 0;
    int value = 0;
    for (const auto& digit : digit_names) {
        const size_t p = FindDigitRight(str, digit);
        if (p != std::string_view::npos && p >= lpos) {
            lpos = p;
            value = digit.value;
        }
    }
    return value;
}

int Solve_2(const std::filesystem::path& input)
{
    int sum = 0;
    for (auto line : ReadLines(input) | stdv::filter(not_empty))
    {
        const int d1 = FindDigitLeft(line);
        const int d2 = FindDigitRight(line);
        const int value = d1 * 10 + d2;
        sum += value;
    }
    return sum;
}