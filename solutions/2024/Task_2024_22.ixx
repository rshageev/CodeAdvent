module;
#include <scn/scan.h>

export module AoC_2024.Day22;

import std;
import utils;

export uint64 Solve_1(const std::filesystem::path& input);
export uint64 Solve_2(const std::filesystem::path& input);

module : private;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<uint64> res;
        for (std::string_view line : ReadLines(input)) {
            res.push_back(Read<uint64>(line));
        }
        return res;
    }

    uint64 Next(uint64 secret)
    {
        secret = (secret ^ (secret << 6)) % 16777216;
        secret = (secret ^ (secret >> 5)) % 16777216;
        secret = (secret ^ (secret << 11)) % 16777216;
        return secret;
    }

    uint64 Next(uint64 secret, int steps)
    {
        for (int i = 0; i < steps; ++i) {
            secret = Next(secret);
        }
        return secret;
    }

    using Combo = std::array<int, 4>;

    auto CalcPrices(uint64 secret, int n)
    {
        std::vector<std::pair<int, int>> prices;
        int prev_p = secret % 10;
        for (int i = 0; i < n; ++i) {
            int new_p = secret % 10;
            prices.emplace_back(new_p, new_p - prev_p);
            secret = Next(secret);
            prev_p = new_p;
        }
        return prices;
    }

    bool ComboMatches(std::span<const std::pair<int, int>> prices, Combo c, size_t idx)
    {
        for (size_t i = 0; i < 4; ++i) {
            auto pidx = idx + i - 3;
            if (prices[pidx].second != c[i])
                return false;
        }
        return true;
    }

    int ProcessBuyer(std::span<const std::pair<int, int>> prices, Combo c)
    {
        for (size_t idx = 0; idx < prices.size(); ++idx) {
            if (ComboMatches(prices, c, idx)) {
                return prices[idx].first;
            }
        }
        return 0;
    }

    int ProcessAllBuyers(const std::vector<std::vector<std::pair<int, int>>>& all_buyers, Combo c)
    {
        int total_bought = 0;
        for (const auto& buyer_prices : all_buyers) {
            total_bought += ProcessBuyer(buyer_prices, c);
        }
        return total_bought;
    }
}

uint64 Solve_1(const std::filesystem::path& input)
{
    uint64 res = 0;
    for (uint64 sn : LoadData(input)) {
        res += Next(sn, 2000);
    }
    return res;
}

uint64 Solve_2(const std::filesystem::path& input)
{
    const auto secrets = LoadData(input);

    std::vector<std::vector<std::pair<int, int>>> all_buyers;
    for (uint64 sn : secrets) {
        all_buyers.push_back(CalcPrices(sn, 2001));
    }

    std::vector<std::jthread> threads;
    std::atomic<int> result = 0;

    for (int x = -9; x < 10; ++x)
    {
        threads.emplace_back([&, x](){
            int new_max = 0;
            for (int y = -9; y < 10; ++y) {
                for (int z = -9; z < 10; ++z) {
                    for (int w = -9; w < 10; ++w) {
                        Combo c = { x, y, z, w };
                        int bought = ProcessAllBuyers(all_buyers, c);
                        new_max = std::max(bought, new_max);
                    }
                }
            }
            int prev_max = result.load();
            do {
                new_max = std::max(prev_max, new_max);
            }
            while (!result.compare_exchange_weak(prev_max, new_max));
        });
    }
    threads.clear();

    return result;
}