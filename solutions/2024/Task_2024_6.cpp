#include "Runner.h"

import std;
import utils;

namespace
{
    struct Cell {
        char type = '.';
        Direction visited = 0;

        bool operator==(const Cell&) const = default;
    };

    bool Walk(Array2D<Cell>& data, Point gpos, Direction gdir = Dir::Up)
    {
        while (data.Contains(gpos) && ((data[gpos].visited & gdir) != gdir))
        {
            data[gpos].visited |= gdir;

            Point next = MovePoint(gpos, gdir);
            while (data.Contains(next) && data[next].type == '#') {
                gdir = RotateRight(gdir);
                next = MovePoint(gpos, gdir);
            }
            gpos = next;  
        }
        return data.Contains(gpos);
    }

    size_t Solve_1(const std::filesystem::path& input)
    {
        auto data = ReadArray2D(input, make<Cell>);
        const Point start = FindInArray2D(data, Cell{ '^' });
        Walk(data, start);
        return stdr::count_if(data, &Cell::visited);
    }

    int Solve_2(const std::filesystem::path& input)
    {
        auto data = ReadArray2D(input, make<Cell>);
        const Point start = FindInArray2D(data, Cell{ '^' });

        Walk(data, start);

        std::vector<Point> path;
        for (Point pos : to_cell_coords(data)) {
            if (data[pos].visited && data[pos].type == '.') {
                path.push_back(pos);
            }
        }

        int result = 0;
        for (Point pos : path) {
            for (auto& cell : data) cell.visited = 0;

            data[pos].type = '#';
            if (Walk(data, start)) {
                ++result;
            }
            data[pos].type = '.';
        }

        return result;
    }

    REGISTER_SOLUTION(2024, 6, 1, Solve_1);
    REGISTER_SOLUTION(2024, 6, 2, Solve_2);
}
