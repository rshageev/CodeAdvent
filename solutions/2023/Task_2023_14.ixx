export module AoC_2023.Day14;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

Array2D<char> LoadData(const std::filesystem::path& input)
{
    const auto text = ReadText(input);
    auto data = Array2DFromString(text, [](char ch) { return ch; });
    return data;
}

void MoveRock(Array2D<char>& arr, Point rock_pos, Point dir)
{
    Point new_pos = rock_pos + dir;
    while (Contains(arr.Area(), new_pos) && arr[new_pos] == '.') {
        new_pos += dir;
    }
    std::swap(arr[rock_pos], arr[new_pos - dir]);
}

void TiltUp(Array2D<char>& arr)
{
    for (int x : arr.ColumnIndices()) {
        for (int y : arr.RowIndices() | stdv::reverse | stdv::drop(1)) {
            if (arr[{x, y}] == 'O') {
                MoveRock(arr, Point{ x, y }, Point{ 0, 1 });
            }
        }
    }
}

void TiltDown(Array2D<char>& arr)
{
    for (int x : arr.ColumnIndices()) {
        for (int y : arr.RowIndices() | stdv::drop(1)) {
            if (arr[{x, y}] == 'O') {
                MoveRock(arr, Point{ x, y }, Point{ 0, -1 });
            }
        }
    }
}

void TiltLeft(Array2D<char>& arr)
{
    for (int y : arr.RowIndices()) {
        for (int x : arr.ColumnIndices() | stdv::drop(1)) {
            if (arr[{x, y}] == 'O') {
                MoveRock(arr, Point{ x, y }, Point{ -1, 0 });
            }
        }
    }
}

void TiltRight(Array2D<char>& arr)
{
    for (int y : arr.RowIndices()) {
        for (int x : arr.ColumnIndices() | stdv::reverse | stdv::drop(1)) {
            if (arr[{x, y}] == 'O') {
                MoveRock(arr, Point{ x, y }, Point{ 1, 0 });
            }
        }
    }
}

int CalcWeight(const Array2D<char>& arr)
{
    int w = 0;
    for (Point pt : to_cell_coords(arr)) {
        if (arr[pt] == 'O') {
            w += (pt.y + 1);
        }
    }
    return w;
}

int Solve_1(const std::filesystem::path& input)
{
    auto data = LoadData(input);
    TiltUp(data);
    return CalcWeight(data);
}

void Spin(Array2D<char>& data)
{
    TiltUp(data);
    TiltLeft(data);
    TiltDown(data);
    TiltRight(data);
}

int Solve_2(const std::filesystem::path& input)
{
    auto data = LoadData(input);

    size_t rem_cycles = 1'000'000'000;
    size_t cycle_len = rem_cycles;
    std::vector<Array2D<char>> cache = { data };

    while (rem_cycles > 0) {
        Spin(data);
        --rem_cycles;

        auto itr = stdr::find(cache, data);
        if (itr != stdr::end(cache)) {
            cycle_len = std::distance(itr, stdr::end(cache));
            break;
        } else {
            cache.push_back(data);
        }
    }

    rem_cycles = rem_cycles % cycle_len;
    for (size_t i = 0; i < rem_cycles; ++i) {
        Spin(data);
    }

    return CalcWeight(data);
}