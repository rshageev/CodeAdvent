export module AoC_2021.Day25;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

Dir CharToDir(char ch)
{
    switch (ch) {
    case '-': return Dir::None;
    case '>': return Dir::Right;
    case 'v': return Dir::Down;
    }
    return Dir::None;
}

Point WrapPos(Point pos, Rect area)
{
    while (pos.x >= area.x + area.w) pos.x -= area.w;
    //while (pos.x < area.x) pos.x += area.w;
    //while (pos.y >= area.y + area.h) pos.y -= area.h;
    while (pos.y < area.y) pos.y += area.h;
    return pos;
}

Array2D<Dir> LoadField(const std::filesystem::path& input)
{
    const auto file_data = ReadText(input);
    return Array2DFromString(file_data, CharToDir);
}

bool UpdateHerd(const Array2D<Dir>& old_state, Array2D<Dir>& new_state, Dir dir)
{
    const auto offset = DirToOffset(dir);
    const auto area = old_state.Area();
    bool moved_any = false;

    new_state = old_state;

    for (Point old_pos : to_cell_coords(area))
    {
        if (old_state[old_pos] == dir)
        {
            const auto new_pos = WrapPos(old_pos + offset, area);
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
    auto field = LoadField(input);
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

int Solve_2(const std::filesystem::path& input)
{
    return 0;
}