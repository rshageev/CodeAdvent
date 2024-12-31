#include "Runner.h"

import std;
import utils;

namespace
{
    bool AllDifferent(auto&& rng)
    {
        std::bitset<256> b;
        for (char ch : rng) {
            b.set(std::bit_cast<uint8>(ch));
        }
        return b.count() == stdr::size(rng);
    }

    size_t Solve(size_t seq_size, const std::filesystem::path& input)
    {
        for (auto [idx, chunk] : ReadText(input) | stdv::slide(seq_size) | stdv::enumerate) {
            if (AllDifferent(chunk)) {
                return idx + seq_size;
            }
        }
        return 0;
    }

    REGISTER_SOLUTION(2022, 6, 1, std::bind_front(Solve, 4));
    REGISTER_SOLUTION(2022, 6, 2, std::bind_front(Solve, 14));
}
