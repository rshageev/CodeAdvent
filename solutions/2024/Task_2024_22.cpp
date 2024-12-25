#include "Runner.h"

import std;
import utils;

namespace
{
    uint64 Next(uint64 secret)
    {
        secret = (secret ^ (secret << 6)) % 16777216;
        secret = (secret ^ (secret >> 5)) % 16777216;
        secret = (secret ^ (secret << 11)) % 16777216;
        return secret;
    }

    using ChangeSeq = std::array<int8, 4>;

    struct ChangeSeqHash {
        size_t operator() (ChangeSeq seq) const {
            return std::hash<uint32>{}(std::bit_cast<uint32>(seq));
        }
    };
    using BuyerPrices = std::unordered_map<ChangeSeq, int, ChangeSeqHash>;

    auto ProcessBuyerPrices(uint64 secret, int count = 2000)
    {
        BuyerPrices res;
        ChangeSeq seq;

        int prev_price = secret % 10;
        for (int i = 0; i < count; ++i) {
            secret = Next(secret);
            stdr::shift_left(seq, 1);
            int new_price = secret % 10;
            seq[3] = new_price - prev_price;
            prev_price = new_price;
            if (i >= 4) {
                res.emplace(seq, prev_price);
            }
        }
        return res;
    }

    uint64 Solve_1(const std::filesystem::path& input)
    {
        uint64 res = 0;
        for (uint64 sn : ReadLines(input) | stdv::transform(to_uint64)) {
            for (int i = 0; i < 2000; ++i) {
                sn = Next(sn);
            }
            res += sn;
        }
        return res;
    }

    int Solve_2(const std::filesystem::path& input)
    {
        // calc [sequence,price] map for each buyer and combine them into one
        BuyerPrices all_prices;
        for (uint64 secret : ReadLines(input) | stdv::transform(to_uint64)) {
            for (auto [seq, p] : ProcessBuyerPrices(secret)) {
                all_prices[seq] += p;
            }
        }

        return stdr::max_element(all_prices, std::less{}, tuple_get<1>)->second;
    }

    REGISTER_SOLUTION(2024, 22, 1, Solve_1);
    REGISTER_SOLUTION(2024, 22, 2, Solve_2);
}