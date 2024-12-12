export module AoC_2024.Day12;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

void FillRegion(const Array2D<char>& map, Point start, int id, Array2D<int>& out)
{
    out[start] = id;
    std::queue<Point> q;
    q.push(start);
    char ch = map[start];

    while (!q.empty()) {
        Point pos = q.front();
        q.pop();

        for (auto [dir, off] : Neighbours4) {
            Point np = pos + off;
            if (map.Contains(np) && map[np] == ch && out[np] < 0) {
                out[np] = id;
                q.push(np);
            }
        }
    }
}

std::pair<Array2D<int>, int> LoadRegions(const std::filesystem::path& input)
{
    auto map = ReadArray2D(input);
    Array2D<int> regions(map.Area(), -1);

    int id = 0;
    for (Point pt : to_cell_coords(map)) {
        if (regions[pt] < 0) {
            FillRegion(map, pt, id++, regions);
        }
    }
    return { regions,id };
}

int RegionPerimeter(const Array2D<int>& regions, int id)
{
    int res = 0;
    for (Point pt : to_cell_coords(regions)) {
        if (regions[pt] == id) {
            for (auto [dir, off] : Neighbours4) {
                Point np = pt + off;
                if (!regions.Contains(np) || regions[np] != id) {
                    ++res;
                }
            }
        }
    }
    return res;
}

int CalcCellCorners(const Array2D<int>& regions, int id, Point pos)
{
    constexpr Point N8[8] = {
        {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}
    };
    std::uint8_t sig = 0;
    for (Point off : N8) {
        Point np = pos + off;
        sig *= 2;
        if (regions.Contains(np) && regions[np] == id) {
            sig++;
        }
    }

    int res = 0;
    
    // outer corners
    if ((sig & 0b01010000) == 0) ++res;
    if ((sig & 0b00010100) == 0) ++res;
    if ((sig & 0b00000101) == 0) ++res;
    if ((sig & 0b01000001) == 0) ++res;

    // inner
    if ((sig & 0b01110000) == 0b01010000) ++res;
    if ((sig & 0b00011100) == 0b00010100) ++res;
    if ((sig & 0b00000111) == 0b00000101) ++res;
    if ((sig & 0b11000001) == 0b01000001) ++res;

    return res;
}

int CountCorners(const Array2D<int>& regions, int id)
{
    int res = 0;
    for (Point pos : to_cell_coords(regions)) {
        if (regions[pos] == id) {
            res += CalcCellCorners(regions, id, pos);
        }
    }
    return res;
}

int64 Solve_1(const std::filesystem::path& input)
{
    const auto [regions, max_id] = LoadRegions(input);

    int64 res = 0;
    for (int id = 0; id < max_id; ++id) {
        int64 area = stdr::count(regions, id);
        res += area * RegionPerimeter(regions, id);
    }
    return res;
}

int64 Solve_2(const std::filesystem::path& input)
{
    const auto [regions, max_id] = LoadRegions(input);

    int64 res = 0;
    for (int id = 0; id < max_id; ++id) {
        int64 area = stdr::count(regions, id);;
        int sides = CountCorners(regions, id);
        res += area * sides;
    }
    return res;
}