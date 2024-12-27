#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    struct Range {
        int start, end;
    };

    constexpr bool Contains(Range r1, Range r2)
    {
        return (r2.start >= r1.start) && (r2.end <= r1.end);
    }

    constexpr bool Intersects(Range r1, Range r2)
    {
        return (r1.start <= r2.end) && (r2.start <= r1.end);
    }

    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<std::pair<Range, Range>> ranges;
        for (std::string_view line : ReadLines(input)) {
            if (auto res = scn::scan<int, int, int, int>(line, "{}-{},{}-{}")) {
                auto [v1, v2, v3, v4] = res->values();
                ranges.emplace_back(Range{ v1,v2 }, Range{ v3, v4 });
            }
        }
        return ranges;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        return stdr::count_if(LoadData(input), [](auto r) {
            return Contains(r.first, r.second) || Contains(r.second, r.first);
        });
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        return stdr::count_if(LoadData(input), apply_to_pair{ Intersects });
    }

    REGISTER_SOLUTION(2022, 4, 1, Solve_1);
    REGISTER_SOLUTION(2022, 4, 2, Solve_2);
}
