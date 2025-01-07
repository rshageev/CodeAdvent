#include "Runner.h"

import std;
import utils;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {
        const auto lines = ReadLines(input);

        std::unordered_set<std::string> names;
        for (std::string_view line : lines) {
            names.emplace(ReadWord(line));
        }

        const int n = (int)names.size();
        Array2D<int> mat(n, n, 0);

        int from = 0;
        int to = 1;
        for (std::string_view line : lines) {
            const auto first_digit = line.find_first_of("123456789");
            int val = to_int(line.substr(first_digit));
            if (line.contains("lose")) val = -val;

            mat[{from, to}] = val;

            ++to;
            if (to == from) ++to;
            if (to >= n) {
                ++from;
                to = 0;
            }
        }

        return mat;
    }

    int CalcHappiness(const Array2D<int>& map, std::span<const int> persons)
    {
        int result = 0;
        const auto size = persons.size();
        for (size_t i = 0; i < size; ++i) {
            int p1 = persons[i];
            int p2 = persons[(i + 1) % size];
            result += map[{p1, p2}];
            result += map[{p2, p1}];
        }
        return result;
    }

    int Solve(const Array2D<int>& hmap)
    {
        std::vector<int> p;
        p.assign_range(stdv::iota(0, hmap.Width()));

        int best_res = CalcHappiness(hmap, p);
        while (stdr::next_permutation(p).found) {
            best_res = std::max(best_res, CalcHappiness(hmap, p));
        }
        return best_res;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        const auto map = LoadData(input);
        return Solve(map);
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        const auto map = LoadData(input);
        Array2D<int> map2(map.Width() + 1, map.Height() + 1, 0);
        CopyArray2D(map, map2);
        return Solve(map2);
    }

    REGISTER_SOLUTION(2015, 13, 1, Solve_1);
    REGISTER_SOLUTION(2015, 13, 2, Solve_2);
}
