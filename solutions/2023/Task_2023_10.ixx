export module AoC_2023.Day10;

import std;
import utils;

export size_t Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

struct Tube
{
    char ch;
    Direction dirs;
};

using Path = std::vector<std::pair<Point, Direction>>;

constexpr Tube tubes[] = {
    { '-', Dir::Left | Dir::Right },
    { '|', Dir::Up | Dir::Down },
    { 'F', Dir::Right | Dir::Down },
    { 'L', Dir::Right |  Dir::Up },
    { '7', Dir::Left |  Dir::Down },
    { 'J', Dir::Left | Dir::Up },
};

constexpr std::optional<Tube> GetTube(char tube_char)
{
    auto itr = stdr::find(tubes, tube_char, &Tube::ch);
    if (itr != stdr::end(tubes)) {
        return *itr;
    }
    return std::nullopt;
}

Array2D<char> LoadMap(const std::filesystem::path& input)
{
    const auto data = ReadText(input);
    return Array2DFromString(data, [](char ch) { return ch; });
}

std::optional<Direction> GetTubeOutput(const Tube tube, Direction in_dir)
{
    const auto opp_dir = Opposite(in_dir);
    if (tube.dirs & opp_dir)
        return tube.dirs & (~opp_dir);
    return {};
}

auto TraceTube(const Array2D<char>& map, Point current, Direction path_dir)
{
    Path path;

    while (true)
    {
        path.emplace_back(current, path_dir);

        Point next = MovePoint(current, path_dir);

        const auto tube = GetTube(map[next]);
        if (!tube) break; // next cell is not tube

        auto next_dir = GetTubeOutput(*tube, path_dir);
        if (!next_dir) break; // next cell is not connected to current

        current = next;
        path_dir = *next_dir;
    }

    return path;
}

std::optional<Path> FindLoop(const Array2D<char>& map)
{
    const Point start = FindInArray2D(map, 'S');

    for (const auto dir : { Dir::Left, Dir::Right, Dir::Up, Dir::Down })
    {
        auto path = TraceTube(map, start, dir);
        if (MovePoint(path.back().first, path.back().second) == start) {
            return path;
        }
    }

    return {};
}

int CountInside(Rect area, const Path& path)
{
    // Find path directions in each cell

    Array2D<Direction> path_dirs(area, Dir::None);
    for (size_t idx = 0; idx < path.size(); ++idx)
    {
        const Point current = path[idx].first;
        const Point prev = path[(idx + path.size() - 1) % path.size()].first;
        const Point next = path[(idx + 1) % path.size()].first;
        if (next.y > current.y || current.y > prev.y) {
            path_dirs[current] = Dir::Up;
        } else if (next.y < current.y || current.y < prev.y) {
            path_dirs[current] = Dir::Down;
        } else {
            path_dirs[current] = Dir::Left;
        }
    }

    // Calculate inside cells
    int count = 0;
    for (int row : path_dirs.RowIndices())
    {
        Direction loop_dir = Dir::None;
        bool inside = false;
        for (auto cell_dir : path_dirs.GetRow(row))
        { 
            if (IsVertical(cell_dir))
            {
                if (cell_dir != loop_dir) {
                    inside = !inside;
                    loop_dir = cell_dir;
                }
            }
            else if (cell_dir == Dir::None)
            {
                if (inside) ++count;
            }
        }
    }

    return count;
}

size_t Solve_1(const std::filesystem::path& input)
{
    const auto map = LoadMap(input);
    const auto loop = FindLoop(map);
    return loop->size() / 2;
}

int Solve_2(const std::filesystem::path& input)
{
    const auto map = LoadMap(input);
    const auto loop = FindLoop(map);
    return CountInside(map.Area(), *loop);
}