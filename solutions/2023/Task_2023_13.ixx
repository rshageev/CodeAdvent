export module AoC_2023.Day13;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

std::vector<Array2D<char>> ReadInput(const std::filesystem::path& input)
{
    std::vector<Array2D<char>> out;

    for (auto map_lines : ReadLineBlocks(input))
    {
        auto arr = Array2DFromLines(map_lines);
        FlipArrayVertical(arr);
        out.push_back(std::move(arr));
    }
    return out;
}

bool CheckReflectionHor(const Array2D<char>& arr, int col, int nudges = 0)
{
    int x1 = col;
    int x2 = col + 1;

    while (x1 >= 0 && x2 < arr.Width())
    {
        int mismatched = 0;
        for (int y = 0; y < arr.Height(); ++y) {
            if (arr[{x1, y}] != arr[{x2, y}]) {
                ++mismatched;
            }
        }

        if (mismatched > nudges) return false;

        nudges -= mismatched;
        --x1;
        ++x2;
    }
    return (nudges == 0);
}

bool CheckReflectionVer(const Array2D<char>& arr, int row, int nudges = 0)
{
    int y1 = row;
    int y2 = row + 1;

    while (y1 >= 0 && y2 < arr.Height())
    {
        int mismatched = 0;
        for (int x = 0; x < arr.Width(); ++x) {
            if (arr[{x, y1}] != arr[{x, y2}]) {
                ++mismatched;
            }
        }
        if (mismatched > nudges) return false;

        nudges -= mismatched;
        --y1;
        ++y2;
    }
    return (nudges == 0);
}

std::pair<int, int> FindSymmetry(const Array2D<char>& arr, int nudges = 0)
{
    int hor = 0;
    int ver = 0;

    for (int i = 0; i < arr.Width() - 1; ++i) {
        if (CheckReflectionHor(arr, i, nudges)) {
            hor = i + 1;
        }
    }

    for (int i = 0; i < arr.Height() - 1; ++i) {
        if (CheckReflectionVer(arr, i, nudges)) {
            ver = i + 1;
        }
    }

    return { hor, ver };
}

int Solve(const std::filesystem::path& input, int nudges)
{
    const auto patterns = ReadInput(input);

    int result = 0;
    for (const auto& ptn : patterns) {
        auto [h, v] = FindSymmetry(ptn, nudges);
        result += (h + v * 100);
    }
    return result;
}

int Solve_1(const std::filesystem::path& input)
{
    return Solve(input, 0);
}

int Solve_2(const std::filesystem::path& input)
{
    return Solve(input, 1);
}