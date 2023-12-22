export module AoC_2023.Day22;

import std;
import utils;

export size_t Solve_1(const std::filesystem::path& input);
export size_t Solve_2(const std::filesystem::path& input);

module : private;

struct Brick
{
    int x1, y1, z1;
    int x2, y2, z2;

    Rect RectXY() const
    {
        return { x1, y1, x2 - x1 + 1, y2 - y1 + 1 };
    }

    bool Contains(int x, int y, int z) const
    {
        return x >= x1 && x <= x2
            && y >= y1 && y <= y2
            && z >= z1 && z <= z2;
    }
};

auto ReadBricks(const std::filesystem::path& input)
{
    std::vector<Brick> bricks;

    for (const auto& line : ReadLines(input) | stdv::filter(not_empty))
    {
        std::string_view str(line);
        int x1 = Read<int>(str); Skip(str, ",");
        int y1 = Read<int>(str); Skip(str, ",");
        int z1 = Read<int>(str); Skip(str, "~");
        int x2 = Read<int>(str); Skip(str, ",");
        int y2 = Read<int>(str); Skip(str, ",");
        int z2 = Read<int>(str);

        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        if (z1 > z2) std::swap(z1, z2);

        bricks.emplace_back(x1, y1, z1, x2, y2, z2);
    }

    return bricks;
}

void FallBricks(std::span<Brick> bricks)
{
    stdr::sort(bricks, std::less{}, &Brick::z1);

    Rect bounds = bricks[0].RectXY();
    for (const auto& b : bricks | stdv::drop(1)) {
        bounds = BoundingRect(bounds, b.RectXY());
    }

    Array2D<int> heights(bounds, 0);

    for (auto& b : bricks)
    {
        int max_h = 0;
        for (Point pt : to_cell_coords(b.RectXY())) {
            max_h = std::max(max_h, heights[pt]);
        }

        const int fall_dist = b.z1 - max_h - 1;
        b.z1 -= fall_dist;
        b.z2 -= fall_dist;

        for (Point pt : to_cell_coords(b.RectXY())) {
            heights[pt] = b.z2;
        }
    }
}

using SupportGraph = std::vector<std::vector<size_t>>;

auto CalcBrickSupports(std::span<const Brick> bricks)
{
    SupportGraph supports(bricks.size());

    for (auto [i, b] : bricks | stdv::enumerate)
    {
        for (size_t j = 0; j < i; ++j) {
            for (Point pt : to_cell_coords(b.RectXY())) {
                if (bricks[j].Contains(pt.x, pt.y, b.z1 - 1)) {
                    supports[i].push_back(j);
                    break;
                }
            }
        }
    }

    return supports;
}

size_t CountFallen(const SupportGraph& supports, size_t idx)
{
    std::vector<bool> falling(supports.size(), false);

    falling[idx] = true;

    auto is_falling = [&](size_t idx) { return falling[idx]; };

    bool new_fallen = true;
    while (new_fallen)
    {
        new_fallen = false;
        for (size_t i = 0; i < supports.size(); ++i)
        {
            if (!is_falling(i) && !supports[i].empty() && stdr::all_of(supports[i], is_falling))
            {
                falling[i] = true;
                new_fallen = true;
            };
        }
    }

    return stdr::count(falling, true) - 1;
}

size_t Solve_1(const std::filesystem::path& input)
{
    auto bricks = ReadBricks(input);
    FallBricks(bricks);
    const auto supports = CalcBrickSupports(bricks);

    std::set<size_t> nd;
    for (const auto& s : supports) {
        if (s.size() == 1) {
            nd.insert(s[0]);
        }
    }
    return supports.size() - nd.size();
}

size_t Solve_2(const std::filesystem::path& input)
{
    auto bricks = ReadBricks(input);
    FallBricks(bricks);
    const auto supports = CalcBrickSupports(bricks);

    size_t sum = 0;
    for (size_t i = 0; i < supports.size(); ++i) {
        sum += CountFallen(supports, i);
    }
    return sum;
}