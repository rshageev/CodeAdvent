export module AoC_2024.Day6;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

struct Cell {
    char type = '.';
    Direction visited = 0;

    bool operator==(const Cell&) const = default;
};

bool Walk(auto& data, Point gpos, Direction gdir = Dir::Up)
{
    while (data.Contains(gpos))
    {
        data[gpos].visited |= gdir;

        Point next = MovePoint(gpos, gdir);
        while (data.Contains(next) && data[next].type == '#') {
            gdir = RotateRight(gdir);
            next = MovePoint(gpos, gdir);
        }
        gpos = next;

        if (data.Contains(next) && (data[next].visited & gdir) == gdir) {
            return true;
        }
    }
    return false;
}

int Solve_1(const std::filesystem::path& input)
{
    auto data = ReadArray2D(input, make<Cell>);
    const Point start = FindInArray2D(data, Cell{'^'});
    Walk(data, start);
    return stdr::count_if(data, &Cell::visited);
}

int Solve_2(const std::filesystem::path& input)
{
    auto data = ReadArray2D(input, make<Cell>);
    const Point start = FindInArray2D(data, Cell{'^'});

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
