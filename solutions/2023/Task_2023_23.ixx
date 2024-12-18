export module AoC_2023.Day23;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

void DFS(const Array2D<char>& map, Point pos, Point finish, int step, Array2D<bool>& visited, int& max_path)
{
    if (pos == finish) {
        if (step > max_path) {
            max_path = step;
        }
        return;
    }

    visited[pos] = true;

    auto slope_dir = CharToDir(map[pos]);
    for (auto [dir, off] : Neighbours4)
    {
        const Point npos = pos + off;
        if (map.Contains(npos)
            && !visited[npos]
            && map[npos] != '#'
            && (slope_dir == Dir::None || slope_dir == dir))
        {
            DFS(map, npos, finish, step + 1, visited, max_path);
        }
    }

    visited[pos] = false;
}

int Solve(const Array2D<char>& map)
{
    const auto start = LeftTop(map.Area()) + Point{ 1, 0 };
    const auto finish = RightBottom(map.Area()) - Point{ 1, 0 };
    Array2D<bool> visited{ map.Area(), false };
    int max_path = 0;

    DFS(map, start, finish, 0, visited, max_path);

    return max_path;
}

int Solve_1(const std::filesystem::path& input)
{
    return Solve(ReadArray2D(input));
}

int Solve_2(const std::filesystem::path& input)
{
    // brute force solution, takes 5-10 minutes to run
    return Solve(ReadArray2D(input, [](char ch) { return ch == '#' ? '#' : '.'; }));
}