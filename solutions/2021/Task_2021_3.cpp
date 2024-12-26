#include "Runner.h"

import std;
import utils;

namespace
{
    struct BitIs {
        int bit_pos;
        char bit;

        bool operator()(std::string_view str) const {
            return str[bit_pos] == bit;
        }
    };
    char MCBitAt(std::span<const std::string_view> numbers, int bit_pos)
    {
        const size_t ones = stdr::count_if(numbers, BitIs{ bit_pos, '1' });
        const size_t threshold = (numbers.size() + 1u) / 2u;
        return (ones >= threshold) ? '1' : '0';
    }

    char LCBitAt(std::span<const std::string_view> numbers, int bit_pos)
    {
        const size_t zeroes = stdr::count_if(numbers, BitIs{ bit_pos, '0' });
        const size_t threshold = numbers.size() / 2u;
        return (zeroes <= threshold) ? '0' : '1';
    }

    uint32 BinaryStrToUint(std::string_view str) {
        uint32 res = 0;
        for (char ch : str) {
            res = (res << 1) + (ch - '0');
        }
        return res;
    }

    uint32 PowerConsumption(std::span<const std::string_view> numbers)
    {
        std::string gamma;
        std::string eps;
        for (int bit_pos = 0; bit_pos < std::ssize(numbers[0]); ++bit_pos) {
            gamma.push_back(MCBitAt(numbers, bit_pos));
            eps.push_back(LCBitAt(numbers, bit_pos));
        }
        return BinaryStrToUint(gamma) * BinaryStrToUint(eps);
    }

    uint32 OxygenRating(std::span<const std::string_view> numbers, int bit_pos = 0)
    {
        if (numbers.size() == 1) {
            return BinaryStrToUint(numbers[0]);
        }

        char mc_bit = MCBitAt(numbers, bit_pos);
        auto new_numbers = numbers | stdv::filter(BitIs{ bit_pos, mc_bit }) | stdr::to<std::vector>();
        return OxygenRating(new_numbers, bit_pos + 1);
    }

    uint32 CO2Rating(std::span<const std::string_view> numbers, int bit_pos = 0)
    {
        if (numbers.size() == 1) {
            return BinaryStrToUint(numbers[0]);
        }

        char lc_bit = LCBitAt(numbers, bit_pos);
        auto new_numbers = numbers | stdv::filter(BitIs{ bit_pos, lc_bit }) | stdr::to<std::vector>();
        return CO2Rating(new_numbers, bit_pos + 1);
    }

    int Solve_1(const std::filesystem::path& input)
    {
        const auto lines = ReadLines(input);
        auto lines_view = lines | stdv::transform(make<std::string_view>) | stdr::to<std::vector>();

        return PowerConsumption(lines_view);
    }

    int Solve_2(const std::filesystem::path& input)
    {
        const auto lines = ReadLines(input);
        auto lines_view = lines | stdv::transform(make<std::string_view>) | stdr::to<std::vector>();

        return OxygenRating(lines_view) * CO2Rating(lines_view);
    }

    REGISTER_SOLUTION(2021, 3, 1, Solve_1);
    REGISTER_SOLUTION(2021, 3, 2, Solve_2);
}