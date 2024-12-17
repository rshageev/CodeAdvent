export module AoC_2024.Day15;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

std::string Widen(std::string_view line)
{
    std::string res;
    for (char ch : line) {
        switch (ch) {
        case '@': res.append("@."); break;
        case '.': res.append(".."); break;
        case '#': res.append("##"); break;
        case 'O': res.append("[]"); break;
        }
    }
    return res;
}

auto LoadData(const std::filesystem::path& input, bool wide = false)
{
    auto blocks = ReadLineBlocks(input);

    if (wide) {
        blocks[0] = blocks[0] | stdv::transform(Widen) | stdr::to<std::vector>();
    }

    return std::pair(
        Array2DFromLines(blocks[0]),
        blocks[1] | stdv::join | stdv::transform(char_to_dir{}) | stdr::to<std::vector>()
    );
}

Point MoveRobot(Array2D<char>& map, Point start_pos, Direction dir)
{
    std::vector<Point> to_move;

    Point pos = start_pos;
    for (int i = 0; i < 100; ++i) {        
        if (map[pos] == '.')
            break;
        if (map[pos] == '#')
            return start_pos; // nowhere to move
        to_move.push_back(pos);
        pos = MovePoint(pos, dir);
    }

    for (Point pt : to_move | stdv::reverse) {
        Point np = MovePoint(pt, dir);
        std::swap(map[pt], map[np]);
    }

    return MovePoint(start_pos, dir);
}

Array2D<char> MoveRobot2(const Array2D<char>& map, Point start_pos, Direction dir)
{
    std::set<Point> pts = { start_pos };
    std::set<Point> next;
    auto nmap = map;

    while (!pts.empty())
    {
        for (Point pt : pts)
        {
            Point npt = MovePoint(pt, dir);

            if (map[pt] == '@') {
                // move robot
                nmap[npt] = '@';
                nmap[pt] = '.';
                next.insert(npt);
            }
            else if (map[pt] == '#') {
                // nowhere to move
                return map;
            }
            else if (map[pt] == '[') {
                Point rpt = MovePoint(pt, Dir::Right);
                Point nrpt = MovePoint(npt, Dir::Right);

                // write box to new positions
                nmap[npt] = '[';
                nmap[nrpt] = ']';
                if (!pts.contains(npt)) next.insert(npt);
                if (!pts.contains(nrpt)) next.insert(nrpt);
                // clear current box position
                if (!pts.contains(rpt) && IsVertical(dir)) nmap[rpt] = '.';
            }
            else if (map[pt] == ']') {
                Point lpt = MovePoint(pt, Dir::Left);
                Point nlpt = MovePoint(npt, Dir::Left);

                // write box to new positions
                nmap[npt] = ']';
                nmap[nlpt] = '[';
                if (!pts.contains(npt)) next.insert(npt);
                if (!pts.contains(nlpt)) next.insert(nlpt);
                // clear current box position
                if (!pts.contains(lpt) && IsVertical(dir)) nmap[lpt] = '.';
            }
        }
        pts = std::move(next);
        next.clear();
    }

    return nmap;
}

int64 CalcGPS(const Array2D<char>& map)
{
    int64 res = 0;
    for (Point bp : to_cell_coords(map)) {
        if (map[bp] == 'O' || map[bp] == '[') {
            res += bp.x;
            res += 100 * (map.Height() - bp.y - 1);
        }
    }
    return res;
}

int64 Solve_1(const std::filesystem::path& input)
{
    auto [map, moves] = LoadData(input);

    Point pos = FindInArray2D(map, '@');
    for (auto dir : moves) {
        pos = MoveRobot(map, pos, dir);
    }

    return CalcGPS(map);
}

int64 Solve_2(const std::filesystem::path& input)
{
    auto [map, moves] = LoadData(input, true);

    for (auto dir : moves) {
        Point pos = FindInArray2D(map, '@');
        map = MoveRobot2(map, pos, dir);
    }

    return CalcGPS(map);
}