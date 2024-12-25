module;
#include <scn/scan.h>

export module AoC_2024.Day25;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<std::array<int,5>> keys;
        std::vector<std::array<int,5>> locks;

        for (const auto& lines : ReadLineBlocks(input))
        {
            bool is_key = contains(lines[0], '.');

            std::array<int, 5> cols = { -1,-1,-1,-1,-1 };
            for (const auto& line : lines) {
                for (auto [idx,ch] : line | stdv::enumerate) {
                    if (ch == '#') cols[idx]++;
                }
            }

            if (is_key) {
                keys.push_back(cols);
            } else {
                locks.push_back(cols);
            }
        }

        return std::pair(keys, locks);
    }
}

int64 Solve_1(const std::filesystem::path& input)
{
    auto [keys, locks] = LoadData(input);

    int64 res = 0;
    for (const auto& key : keys) {
        for (const auto& lock : locks) {
            bool fits = stdr::none_of(stdv::zip(key, lock), [](std::pair<int,int> p) {
                return p.first + p.second > 5;
            });
            if (fits) ++res;
        }
    }
    return res;
}

int64 Solve_2(const std::filesystem::path& input)
{
    return 0;
}