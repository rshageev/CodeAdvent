export module AoC_2024.Day20;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

namespace
{
    Array2D<int> CalcDistanceMap(const Array2D<char> map)
    {
        Point start = FindInArray2D(map, 'S');
        Array2D<int> visited(map.Area(), -1);

        std::queue<std::pair<Point, int>> q;
        q.emplace(start, 0);
        visited[start] = 0;

        while (!q.empty()) {
            auto [pos, dist] = q.front();
            q.pop();

            for (auto [dir, off] : Neighbours4) {
                Point np = pos + off;
                if (map.Contains(np) && visited[np] < 0 && map[np] != '#') {
                    q.emplace(np, dist + 1);
                    visited[np] = dist + 1;
                }
            }
        }

        return visited;
    }

    int64 CountCheats(const Array2D<int>& dm, Point start, int req_savings, int cheat_duration)
    {
        const Rect search_rect = Inflated({ start.x, start.y, 1, 1 }, cheat_duration, cheat_duration);

        int64 res = 0;
        for (Point end : to_cell_coords(search_rect)) {
            int dist = std::abs(end.x - start.x) + std::abs(end.y - start.y);
            if (dm.Contains(end) && dist <= cheat_duration) {         
                int saving = (dm[end] - dm[start]) - dist;
                if (saving >= req_savings) {
                    ++res;
                }
            }
        }
        return res;
    }

    int64 Solve(const std::filesystem::path& input, int req_savings, int cheat_duration)
    {
        auto dist_map = CalcDistanceMap(ReadArray2D(input));
        int64 res = 0;
        for (Point cs : to_cell_coords(dist_map)) {
            if (dist_map[cs] >= 0) {
                res += CountCheats(dist_map, cs, req_savings, cheat_duration);
            }
        }
        return res;
    }
}

int64 Solve_1(const std::filesystem::path& input)
{
    return Solve(input, 100, 2);
}

int64 Solve_2(const std::filesystem::path& input)
{   
    return Solve(input, 100, 20);
}