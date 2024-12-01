export module AoC_2020.Day2;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

struct Policy {
    int p1;
    int p2;
    char ch;
};

std::pair<Policy, std::string_view> ParseLine(std::string_view line)
{
    Policy p;
    p.p1 = Read<int>(line);
    Skip(line, "-");
    p.p2 = Read<int>(line);
    Skip(line, " ");
    p.ch = ReadChar(line);
    Skip(line, ": ");

    return { p, line };
}

bool IsPasswordValid(const Policy&p, std::string_view pass)
{
    const auto cnt = stdr::count(pass, p.ch);
    return (cnt >= p.p1) && (cnt <= p.p2);
}

bool IsPasswordValid2(const Policy& p, std::string_view pass)
{
    bool match1 = (pass[p.p1 - 1] == p.ch);
    bool match2 = (pass[p.p2 - 1] == p.ch);
    return match1 != match2;
}

int Solve_1(const std::filesystem::path& input)
{
    int result = 0;
    for (const auto& line : ReadLines(input)) {
        auto [policy, pass] = ParseLine(line);
        if (IsPasswordValid(policy, pass)) {
            ++result;
        }
    }
    return result;
}

int Solve_2(const std::filesystem::path& input)
{
    int result = 0;
    for (const auto& line : ReadLines(input)) {
        auto [policy, pass] = ParseLine(line);
        if (IsPasswordValid2(policy, pass)) {
            ++result;
        }
    }
    return result;
}