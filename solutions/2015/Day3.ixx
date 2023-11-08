export module AoC_2015.Day3;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

Point CharToOffset(char ch)
{
    switch (ch) {
        case '>': return { 1, 0 };
        case '<': return { -1, 0 };
        case '^': return { 0, 1 };
        case 'v': return { 0, -1 };
    }
    return { 0, 0 };
}

int Solve_1(const std::filesystem::path& input)
{
    const auto data = ReadText(input);

    Point current = { 0, 0 };
    std::set<Point> visited = { current };

    for (char ch : data) {
        current += CharToOffset(ch);
        visited.insert(current);
    }
    
    return static_cast<int>(visited.size());
}

int Solve_2(const std::filesystem::path& input)
{
    const auto data = ReadText(input);

    Point current[2];
    size_t cnt_idx = 0;

    std::set<Point> visited = { {0, 0} };

    for (char ch : data) {
        current[cnt_idx] += CharToOffset(ch);
        visited.insert(current[cnt_idx]);
        cnt_idx = (cnt_idx + 1) % std::size(current);
    }

    return static_cast<int>(visited.size());
}