export module AoC_2021.Day2;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

std::pair<std::string_view, int> ParseLine(std::string_view str)
{
    auto dir = ReadWord(str);
    auto val = (Skip(str, " "), ReadInt(str));
    return { dir, val };
}

int Solve_1(const std::filesystem::path& input)
{
    Point pos{ 0, 0 };

    for (const auto& line : ReadLines(input) | stdv::filter(not_empty))
    {
        auto [dir, val] = ParseLine(line);

        if (dir == "forward") pos.x += val;
        if (dir == "up") pos.y -= val;
        if (dir == "down") pos.y += val;
    }
    return pos.x * pos.y;
}

int Solve_2(const std::filesystem::path& input)
{
    Point pos{ 0, 0 };
    int aim = 0;

    for (const auto& line : ReadLines(input) | stdv::filter(not_empty))
    {
        auto [dir, val] = ParseLine(line);

        if (dir == "forward") {
            pos.x += val;
            pos.y += (aim * val);
        }
        if (dir == "up") aim -= val;
        if (dir == "down") aim += val;
    }
    return pos.x * pos.y;
}