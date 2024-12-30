#include "Runner.h"

import std;
import utils;

namespace
{
    /* Part 1 */

    auto CountVowels(std::string_view str)
    {
        static constexpr std::string_view vowels = "aeiou";
        return stdr::count_if(str, [=](char ch) { return vowels.contains(ch); });
    }

    bool HasDoubleLetter(std::string_view str)
    {
        return stdr::any_of(str | stdv::pairwise, apply_to_pair{ std::equal_to{} });
    }

    bool ContainsBannedSubs(std::string_view str)
    {
        static constexpr std::string_view banned[] = { "ab", "cd", "pq", "xy" };
        return stdr::any_of(banned, [=](std::string_view bstr) { return str.contains(bstr); });
    }

    bool IsNice1(std::string_view str)
    {
        return (CountVowels(str) >= 3)
            && HasDoubleLetter(str)
            && !ContainsBannedSubs(str);
    }

    /* Part 2 */

    bool HasTwoPairs(std::string_view str)
    {
        while (str.size() >= 4) {
            if (str.substr(2).contains(str.substr(0, 2))) {
                return true;
            }
            str.remove_prefix(1);
        }
        return false;
    }

    bool HasTriple(std::string_view str)
    {
        return stdr::any_of(str | stdv::slide(3), [](const auto& c) { return c[0] == c[2]; });
    }

    bool IsNice2(std::string_view str)
    {
        return HasTwoPairs(str) && HasTriple(str);
    }


    using IsNicePred = bool (*) (std::string_view);
    auto Solve(IsNicePred is_nice, const std::filesystem::path& input)
    {
        return stdr::count_if(ReadLines(input), is_nice);
    }

    REGISTER_SOLUTION(2015, 5, 1, std::bind_front(Solve, IsNice1));
    REGISTER_SOLUTION(2015, 5, 2, std::bind_front(Solve, IsNice2));
}
