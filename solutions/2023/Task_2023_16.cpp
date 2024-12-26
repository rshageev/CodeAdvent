#include "Runner.h"

import std;
import utils;

namespace
{
    void MoveBeam(std::deque<std::pair<Point, Direction>>& out, Point pos, Direction dir, char cell)
    {
        auto emit = [&out, pos](Direction d) { out.emplace_back(MovePoint(pos, d), d); };

        if (IsHorizontal(dir))
        {
            if (cell == '-' || cell == '.') emit(dir);
            if (cell == '/' || cell == '|') emit(RotateLeft(dir));
            if (cell == '\\' || cell == '|') emit(RotateRight(dir));
        }
        else
        {
            if (cell == '|' || cell == '.') emit(dir);
            if (cell == '/' || cell == '-') emit(RotateRight(dir));
            if (cell == '\\' || cell == '-') emit(RotateLeft(dir));
        }
    }

    Array2D<Direction> TraceBeam(const Array2D<char>& mirrors, Point start_pos, Direction start_dir)
    {
        Array2D<Direction> beam_map(mirrors.Area(), Dir::None);

        std::deque<std::pair<Point, Direction>> queue = { { start_pos, start_dir } };

        while (!queue.empty())
        {
            auto [pos, dir] = queue.front();

            if (beam_map.Contains(pos) && (beam_map[pos] & dir) == 0)
            {
                beam_map[pos] |= dir;
                MoveBeam(queue, pos, dir, mirrors[pos]);
            }

            queue.pop_front();
        }

        return beam_map;
    }

    size_t CountActivated(const Array2D<char>& mirrors, Point start_pos, Direction start_dir)
    {
        const auto beam_map = TraceBeam(mirrors, start_pos, start_dir);
        return stdr::count_if(beam_map, not_equal<Dir::None>);
    }

    size_t Solve_1(const std::filesystem::path& input)
    {
        const auto mirrors = ReadArray2D(input);
        return CountActivated(mirrors, LeftTop(mirrors.Area()), Dir::Right);
    }

    size_t Solve_2(const std::filesystem::path& input)
    {
        const auto mirrors = ReadArray2D(input);

        size_t max_activated = 0;
        for (int x : mirrors.ColumnIndices()) {
            auto v1 = CountActivated(mirrors, Point{ x, 0 }, Dir::Up);
            auto v2 = CountActivated(mirrors, Point{ x, mirrors.Height() - 1 }, Dir::Down);
            max_activated = std::max(max_activated, std::max(v1, v2));
        }
        for (int y : mirrors.RowIndices()) {
            auto v1 = CountActivated(mirrors, Point{ 0, y }, Dir::Right);
            auto v2 = CountActivated(mirrors, Point{ mirrors.Width() - 1, y }, Dir::Left);
            max_activated = std::max(max_activated, std::max(v1, v2));
        }

        return max_activated;
    }

    REGISTER_SOLUTION(2023, 16, 1, Solve_1);
    REGISTER_SOLUTION(2023, 16, 2, Solve_2);
}