#include "Runner.h"

import std;
import utils;

namespace
{
    std::pair<std::string_view, int> ParseLine(std::string_view str)
    {
        auto dir = ReadWord(str);
        auto val = (Skip(str, " "), Read<int>(str));
        return { dir, val };
    }

    int Solve_1(const std::filesystem::path& input)
    {
        Point pos{ 0, 0 };

        for (const auto& line : ReadLines(input))
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

        for (const auto& line : ReadLines(input))
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

    REGISTER_SOLUTION(2021, 2, 1, Solve_1);
    REGISTER_SOLUTION(2021, 2, 2, Solve_2);
}