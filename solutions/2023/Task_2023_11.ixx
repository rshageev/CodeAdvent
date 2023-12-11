export module AoC_2023.Day11;

import std;
import utils;

export size_t Solve_1(const std::filesystem::path& input);
export size_t Solve_2(const std::filesystem::path& input);

module : private;

std::vector<Point> LoadStarPositions(const std::filesystem::path& input)
{
    const auto data = ReadText(input);
    const auto map = Array2DFromString(data, [](char ch) { return ch == '#'; });

    std::vector<Point> stars;
    for (Point pos : to_cell_coords(map)) {
        if (map[pos]) stars.push_back(pos);
    }
    return stars;
}

void Expand(std::span<Point> stars, int factor)
{
    auto expand = [=](int Point::* coord) {
        stdr::sort(stars, std::less{}, coord);
        for (size_t idx = 1; idx < stars.size(); ++idx) {
            if (const auto gap = stars[idx].*coord - stars[idx - 1].*coord - 1; gap > 0) {
                for (size_t i = idx; i < stars.size(); ++i) {
                    stars[i].*coord += gap * (factor - 1);
                }
            }
        }
    };
    expand(&Point::x);
    expand(&Point::y);
}

int Distance(Point p1, Point p2)
{
    return std::abs(p2.x - p1.x) + std::abs(p2.y - p1.y);
}

size_t AllDistances(std::span<const Point> stars)
{
    size_t sum = 0;
    for (size_t i1 = 0; i1 < stars.size() - 1; ++i1) {
        for (size_t i2 = i1 + 1; i2 < stars.size(); ++i2) {
            const auto dist = Distance(stars[i1], stars[i2]);
            sum += static_cast<size_t>(dist);
        }
    }
    return sum;
}

size_t Solve(const std::filesystem::path& input, int factor)
{
    auto stars = LoadStarPositions(input);
    Expand(stars, factor);
    return AllDistances(stars);
}

size_t Solve_1(const std::filesystem::path& input)
{
    return Solve(input, 2);
}

size_t Solve_2(const std::filesystem::path& input)
{
    return Solve(input, 1000000);
}