#include "Runner.h"

import std;
import utils;

namespace
{
    bool UpdateHerd(const Array2D<Direction>& old_state, Array2D<Direction>& new_state, Direction dir)
    {
        const auto offset = DirToOffset(dir);
        const auto area = old_state.Area();
        bool moved_any = false;

        new_state = old_state;

        for (Point old_pos : to_cell_coords(area))
        {
            if (old_state[old_pos] == dir)
            {
                const auto new_pos = WrapPoint(old_pos + offset, area);
                if (old_state[new_pos] == Dir::None) {
                    new_state[new_pos] = dir;
                    new_state[old_pos] = Dir::None;
                    moved_any = true;
                }
            }
        }

        return moved_any;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        auto field = ReadArray2D(input, char_to_dir{ "<>^v" });
        auto field2 = field;

        int step_count = 0;
        bool moving = true;
        while (moving)
        {
            bool moved_right = UpdateHerd(field, field2, Dir::Right);
            bool moved_down = UpdateHerd(field2, field, Dir::Down);
            moving = moved_right || moved_down;
            ++step_count;
        }

        return step_count;
    }

    REGISTER_SOLUTION(2021, 25, 1, Solve_1);
}