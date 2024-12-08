module;
#include <scn/scan.h>

export module AoC_2024.Day8;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

void CalcAntinodes(Array2D<char>& data, std::span<const Point> antennas)
{
    for (size_t i = 0; i < antennas.size(); ++i) {
        for (size_t j = i + 1; j < antennas.size(); ++j) {
            Point off = antennas[j] - antennas[i];
            Point p1 = antennas[j] + off;
            Point p2 = antennas[i] - off;
            if (data.Contains(p1)) data[p1] = '#';
            if (data.Contains(p2)) data[p2] = '#';
        }
    }
}

void CalcAntinodes2(Array2D<char>& data, std::span<const Point> antennas)
{
    for (size_t i = 0; i < antennas.size(); ++i) {
        for (size_t j = i + 1; j < antennas.size(); ++j) {
            Point off = antennas[j] - antennas[i];
            Point p;
            p = antennas[j];
            while (data.Contains(p)) {
                data[p] = '#';
                p += off;
            }
            p = antennas[i];
            while (data.Contains(p)) {
                data[p] = '#';
                p -= off;
            }
        }
    }
}

int64 Solve(const std::filesystem::path& input, auto calcAntinodes)
{
    auto data = ReadArray2D(input);

    std::map<char, std::vector<Point>> antennas;
    for (Point pos : to_cell_coords(data)) {
        if (data[pos] != '.') {
            antennas[data[pos]].push_back(pos);
        }
    }

    for (const auto& [freq, vec] : antennas) {
        calcAntinodes(data, vec);
    }

    return stdr::count(data, '#');
}

int64 Solve_1(const std::filesystem::path& input)
{
    return Solve(input, CalcAntinodes);
}

int64 Solve_2(const std::filesystem::path& input)
{
    return Solve(input, CalcAntinodes2);
}