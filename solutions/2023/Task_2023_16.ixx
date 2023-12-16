export module AoC_2023.Day16;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

std::vector<std::pair<Point, Direction>> MoveBeam(Point pos, Direction dir, char cell)
{
    std::vector<std::pair<Point, Direction>> out;
    if (cell == '.')
    {
        out.emplace_back(pos, dir);
    }
    else if (cell == '-')
    {
        if (IsHorizontal(dir)) {
            out.emplace_back(pos, dir);
        }
        else {
            out.emplace_back(pos, Dir::Left);
            out.emplace_back(pos, Dir::Right);
        }
    }
    else if (cell == '|')
    {
        if (IsVertical(dir)) {
            out.emplace_back(pos, dir);
        }
        else {
            out.emplace_back(pos, Dir::Up);
            out.emplace_back(pos, Dir::Down);
        }
    }
    else if (cell == '/')
    {
        if (dir == Dir::Right) out.emplace_back(pos, Dir::Up);
        if (dir == Dir::Left) out.emplace_back(pos, Dir::Down);
        if (dir == Dir::Up) out.emplace_back(pos, Dir::Right);
        if (dir == Dir::Down) out.emplace_back(pos, Dir::Left);
    }
    else if (cell == '\\')
    {
        if (dir == Dir::Right) out.emplace_back(pos, Dir::Down);
        if (dir == Dir::Left) out.emplace_back(pos, Dir::Up);
        if (dir == Dir::Up) out.emplace_back(pos, Dir::Left);
        if (dir == Dir::Down) out.emplace_back(pos, Dir::Right);
    }

    for (auto& [pos, dir] : out) {
        pos = MovePoint(pos, dir);
    }
    return out;
}

auto LoadMap(const std::filesystem::path& input)
{
    const auto text = ReadText(input);
    return Array2DFromString(text, [](char ch) { return ch; });
}

Array2D<Direction> TraceBeam(const Array2D<char>& mirrors, Point start_pos, Direction start_dir)
{
    Array2D<Direction> beam_map(mirrors.Area(), Dir::None);

    std::vector<std::pair<Point, Direction>> cnt;
    std::vector<std::pair<Point, Direction>> next;
    cnt.emplace_back(start_pos, start_dir);

    
    while (!cnt.empty())
    {
        next.clear();

        for (auto [pos, dir] : cnt)
        {
            
            if (Contains(beam_map.Area(), pos))
            {       
                auto& bm = beam_map[pos];
                if ((bm & dir) == 0)
                {
                    // set output map
                    bm = bm | dir;

                    // transform
                    auto next_step = MoveBeam(pos, dir, mirrors[pos]);
                    next.insert(next.end(), next_step.begin(), next_step.end());
                } 
            }
        }

        cnt = std::move(next);
    }

    return beam_map;
}

int CountActivated(const Array2D<Direction>& beam_map)
{
    int count = 0;
    for (auto dir : beam_map) {
        if (dir != Dir::None) {
            ++count;
        }
    }
    return count;
}

int CountActivated(const Array2D<char>& mirrors, Point start_pos, Direction start_dir)
{
    const auto beam_map = TraceBeam(mirrors, start_pos, start_dir);
    return CountActivated(beam_map);
}

int Solve_1(const std::filesystem::path& input)
{
    const auto mirrors = LoadMap(input);
    return CountActivated(mirrors, LeftTop(mirrors.Area()), Dir::Right);
}

int Solve_2(const std::filesystem::path& input)
{
    const auto mirrors = LoadMap(input);

    int max_activated = 0;
    for (int x : mirrors.ColumnIndices()) {
        int v1 = CountActivated(mirrors, Point{ x, 0 }, Dir::Up);
        int v2 = CountActivated(mirrors, Point{ x, mirrors.Height() - 1 }, Dir::Down);
        max_activated = std::max(max_activated, std::max(v1, v2));
    }
    for (int y : mirrors.RowIndices()) {
        int v1 = CountActivated(mirrors, Point{ 0, y }, Dir::Right);
        int v2 = CountActivated(mirrors, Point{ mirrors.Width() - 1, y}, Dir::Left);
        max_activated = std::max(max_activated, std::max(v1, v2));
    }
    return max_activated;
}