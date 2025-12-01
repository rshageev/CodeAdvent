#include "Runner.h"

import std;
import utils;

namespace
{
    auto Solve_1(const std::filesystem::path& input)
    {
        int count = 0;
        int dial = 50;
        for (std::string_view line : ReadLines(input))
        {
            char dir = line[0];
            int angle = to_int(line.substr(1));

            if (dir == 'L') {
                dial -= angle;
            } else {
                dial += angle;
            }
            dial = WrapCoord(dial, 0, 100);
            if (dial == 0)
                ++count;
        }
        return count;
    }

    auto Solve_2_Dumb(const std::filesystem::path& input)
    {
        int count = 0;
        int dial = 50;
        for (std::string_view line : ReadLines(input))
        {
            char dir = line[0];
            int angle = to_int(line.substr(1));
            
            for (int i = 0; i < angle; ++i) {
                if (dir == 'L') {
                    --dial;
                } else {
                    ++dial;
                }
                dial = (dial + 100) % 100;
                if (dial == 0)
                    ++count;
            }
        }
        return count;
    }

    auto Solve_2_Smart(const std::filesystem::path& input)
    {
        int count = 0;
        int dial = 50;
        for (std::string_view line : ReadLines(input))
        {
            char dir = line[0];
            int angle = to_int(line.substr(1));

            int prev_dial = dial;
            if (dir == 'L') {
                dial -= angle;
            } else {
                dial += angle;
            }

            int clicks = 0;
            if (dial > 99) {
                clicks = dial / 100;
            } else if (dial <= 0) {
                clicks = (-dial) / 100;
                if (prev_dial != 0)
                    ++clicks;
            }
            count += clicks;

            dial = WrapCoord(dial, 0, 100);
        }
        return count;
    }

    REGISTER_SOLUTION(2025, 1, 1, Solve_1);
    REGISTER_SOLUTION(2025, 1, 2, Solve_2_Dumb, "dumb");
    REGISTER_SOLUTION(2025, 1, 2, Solve_2_Smart, "smart");
}
