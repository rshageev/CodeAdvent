#include "Runner.h"

import std;
import utils;

namespace
{
    void IncreasePass(std::string& pass)
    {
        for (char& ch : pass | stdv::reverse) {
            if (++ch > 'z') {
                ch = 'a';
            } else {
                break;
            }
        }
    }

    bool HasIncreasingTriple(std::string_view pass)
    {
        for (auto c : pass | stdv::slide(3)) {
            if (c[1] == c[0] + 1 && c[2] == c[1] + 1) {
                return true;
            }
        }
        return false;
    }

    bool HasBannedLetters(std::string_view pass)
    {
        return pass.find_first_of("iol") != std::string_view::npos;
    }

    bool HasDoubleLetters(std::string_view pass)
    {
        int count = 0;
        for (char ch = 'a'; ch <= 'z' && count < 2; ++ch) {
            char p[2] = { ch, ch };
            if (pass.contains(std::string_view(p, 2))) {
                ++count;
            }
        }
        return count >= 2;
    }

    bool PassCorrect(std::string_view pass)
    {
        return !HasBannedLetters(pass)
            && HasIncreasingTriple(pass)
            && HasDoubleLetters(pass);
    }

    std::string NextPass(std::string_view pass)
    {
        std::string p(pass);
        do { IncreasePass(p); } while (!PassCorrect(p));
        return p;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        return NextPass(ReadText(input));
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        return NextPass(NextPass(ReadText(input)));
    }

    REGISTER_SOLUTION(2015, 11, 1, Solve_1);
    REGISTER_SOLUTION(2015, 11, 2, Solve_2);
}
