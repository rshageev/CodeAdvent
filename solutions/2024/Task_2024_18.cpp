#include "Runner.h"

import std;
import utils;

namespace
{
    std::vector<Point> ReadPositions(const std::filesystem::path& input)
    {
        std::vector<Point> pts;
        for (std::string_view line : ReadLines(input)) {
            Point pos;
            Parse{ line }.Read(pos.x).Skip(",").Read(pos.y);
            pts.push_back(pos);
        }
        return pts;
    }

    int FindPath(const Array2D<char>& map)
    {
        Point start = { 0, 0 };
        Point end = { map.Width() - 1, map.Height() - 1 };
        Array2D<bool> visited(map.Area(), false);

        std::queue<std::pair<Point, int>> q;
        q.emplace(start, 0);
        visited[start] = true;

        while (!q.empty()) {
            auto [pos, dist] = q.front();
            q.pop();

            if (pos == end) return dist;

            for (auto [dir, off] : Neighbours4) {
                Point np = pos + off;
                if (map.Contains(np) && !visited[np] && map[np] == '.') {
                    q.emplace(np, dist + 1);
                    visited[np] = true;
                }
            }
        }

        return 0;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        const auto bytes = ReadPositions(input);

        bool is_test = (input.filename() == "test.txt");
        int size = is_test ? 7 : 71;
        int count = is_test ? 12 : 1024;

        Array2D<char> map(size, size, '.');
        for (Point pos : bytes | stdv::take(count)) {
            if (map.Contains(pos)) map[pos] = '#';
        }

        return FindPath(map);
    }

    std::string Solve_2(const std::filesystem::path& input)
    {
        auto bytes = ReadPositions(input);

        bool is_test = (input.filename() == "test.txt");
        int size = is_test ? 7 : 71;

        Array2D<char> map(size, size, '.');

        for (Point bp : bytes) {
            map[bp] = '#';
            if (FindPath(map) == 0) {
                return std::format("{},{}", bp.x, bp.y);
            }
        }

        return "No result";
    }

    REGISTER_SOLUTION(2024, 18, 1, Solve_1);
    REGISTER_SOLUTION(2024, 18, 2, Solve_2);
}