#include "Runner.h"

import std;
import utils;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {
        const auto lines = ReadLines(input);

        // deduce number of cities from the number of lines in the input, based
        // on the fact that complete graph has N(N-1)/2 edges
        const auto cities = static_cast<int>(0.5f * (1.0f + std::sqrt(1.0f + 8.0f * lines.size())));

        Array2D<int> dist_map(cities, cities, 0);

        int from = 0;
        int to = 1;
        for (std::string_view line : ReadLines(input))
        {
            int dist = to_int(line.substr(line.rfind(' ') + 1));

            dist_map[{from, to}] = dist;
            dist_map[{to, from}] = dist;

            if (++to == cities) {
                to = ++from + 1;
            }
        }

        return dist_map;
    }

    int Distance(const Array2D<int>& dist_map, std::span<const int> path)
    {
        int dist = 0;
        for (auto [from, to] : path | stdv::pairwise) {
            dist += dist_map[{from, to}];
        }
        return dist;
    }

    template<typename Cmp>
    int Solve(const std::filesystem::path& input)
    {
        Cmp cmp{};
        const auto dist_map = LoadData(input);
        const int cities = dist_map.Width();
        std::vector<int> path{ std::from_range, stdv::iota(0, cities) };
        int best_dist = Distance(dist_map, path);
        while (stdr::next_permutation(path).found) {
            int dist = Distance(dist_map, path);
            if (cmp(dist, best_dist)) best_dist = dist;
        }
        return best_dist;
    }

    REGISTER_SOLUTION(2015, 9, 1, Solve<std::less<>>);
    REGISTER_SOLUTION(2015, 9, 2, Solve<std::greater<>>);
}
