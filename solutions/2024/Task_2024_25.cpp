import std;
import utils;

#include "Runner.h"

namespace Day25
{
    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<std::array<int, 5>> keys;
        std::vector<std::array<int, 5>> locks;

        for (const auto& lines : ReadLineBlocks(input))
        {
            bool is_key = contains(lines[0], '.');

            std::array<int, 5> cols = { -1,-1,-1,-1,-1 };
            for (const auto& line : lines) {
                for (auto [idx, ch] : line | stdv::enumerate) {
                    if (ch == '#') cols[idx]++;
                }
            }

            if (is_key) {
                keys.push_back(cols);
            }
            else {
                locks.push_back(cols);
            }
        }

        return std::pair(keys, locks);
    }

    int Solve(const std::filesystem::path& input)
    {
        auto [keys, locks] = LoadData(input);

        int res = 0;
        for (const auto& key : keys) {
            for (const auto& lock : locks) {
                bool fits = stdr::none_of(stdv::zip(key, lock), [](std::pair<int, int> p) {
                    return p.first + p.second > 5;
                    });
                if (fits) ++res;
            }
        }
        return res;
    }

    REGISTER_SOLUTION(2024, 25, 1, Solve);
}

