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

bool Walk(auto& data, Point gpos, Direction gdir)
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

        if (data.Contains(next) && ((data[next].visited & gdir) == gdir)) {
            return true;
        }
    }
    return false;
}

int Solve_1(const std::filesystem::path& input)
{
    auto data = Array2DFromString(ReadText(input), [](char ch) {
        return Cell{ ch };
    });
    Point gpos = FindInArray2D(data, Cell{ '^' });

    Walk(data, gpos, Dir::Up);

    return stdr::count_if(data, [](Cell cell) { return cell.visited != 0; });
}

int Solve_2(const std::filesystem::path& input)
{
    auto data = Array2DFromString(ReadText(input), [](char ch) {
        return Cell{ ch };
        });
    Point gpos = FindInArray2D(data, Cell{ '^' });
    
    int result = 0;
    for (Point opos : to_cell_coords(data)) {
        if (opos != gpos && data[opos].type == '.') {
            auto copy = data;
            copy[opos].type = '#';
            if (Walk(copy, gpos, Dir::Up)) {
                ++result;
            }
        }
    }

    return result;
}
