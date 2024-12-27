#include "Runner.h"

import std;
import utils;

namespace
{
    int Solve_1(const std::filesystem::path& input)
    {
        int res = 0;
        for (std::string_view line : ReadLines(input))
        {
            int op = line[0] - 'A'; // 0 - rock, 1 - paper, 2 - scissors
            int mine = line[2] - 'X';
            int game_res = (mine - op + 4) % 3; // 0 - loss, 1 - draw, 2 - win
            res += (game_res * 3) + (mine + 1);
        }
        return res;
    }

    int Solve_2(const std::filesystem::path& input)
    {
        int res = 0;
        for (std::string_view line : ReadLines(input))
        {
            int op = line[0] - 'A'; // 0 - rock, 1 - paper, 2 - scissors
            int result = line[2] - 'X'; // 0 - loss, 1 - draw, 2 - win
            int mine = (op + result + 2) % 3;
            res += (result * 3) + (mine + 1);
        }
        return res;
    }

    REGISTER_SOLUTION(2022, 2, 1, Solve_1);
    REGISTER_SOLUTION(2022, 2, 2, Solve_2);
}

