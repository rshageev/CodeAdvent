#include "Runner.h"

import std;
import utils;

namespace
{
    using Range = std::pair<uint64, uint64>;

    auto ReadData(const std::filesystem::path& input)
    {
        const auto blocks = ReadLineBlocks(input);

        std::vector<Range> ranges;
        for (std::string_view line : blocks[0]) {
            uint64 first = Read<uint64>(line);
            Skip(line, "-");
            uint64 last = Read<uint64>(line);
            ranges.emplace_back(first, last);
        }

        std::vector<uint64> availableIds;
        for (const auto& line : blocks[1]) {
            availableIds.push_back(to_uint64(line));
        }

        return std::pair(ranges, availableIds);
    }

    constexpr bool InRange(uint64 id, Range rng) {
        return id >= rng.first && id <= rng.second;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        const auto [ranges, ids] = ReadData(input);
        return stdr::count_if(ids, [&](uint64 id) {
            return stdr::any_of(ranges, std::bind_front(InRange, id));
        });
    }

    constexpr bool RangesOverlap(Range r1, Range r2)
    {
        return InRange(r1.first, r2) || InRange(r2.first, r1)
            || InRange(r1.second, r2) || InRange(r2.second, r1);
    }

    std::vector<Range> MergeRanges(const std::vector<Range>& in)
    {
        std::vector<Range> merged;
        for (const auto& r1 : in)
        {
            auto r2 = stdr::find_if(merged, std::bind_front(RangesOverlap, r1));
            if (r2 != merged.end()) {
                r2->first = std::min(r1.first, r2->first);
                r2->second = std::max(r1.second, r2->second);
            } else {
                merged.push_back(r1);
            }
        }
        return merged;
    }

    auto Solve_2_Multipass(const std::filesystem::path& input)
    {
        auto [ranges1, _] = ReadData(input);

        std::vector<Range> ranges2;
        while (ranges1.size() != ranges2.size()) {
            ranges2 = std::move(ranges1);
            ranges1 = MergeRanges(ranges2);
        }

        uint64 sum = 0;
        for (auto [first, last] : ranges1) {
            sum += (last - first + 1);
        }
        return sum;
    }

    auto Solve_2_Sorted(const std::filesystem::path& input)
    {
        auto [ranges, _] = ReadData(input);

        stdr::sort(ranges);

        uint64 sum = 0;
        Range m = ranges[0];
        for (auto r : ranges | stdv::drop(1)) {
            if (r.first <= m.second) {
                m.second = std::max(m.second, r.second);
            } else {
                sum += (m.second - m.first + 1);
                m = r;
            }
        }

        return sum + (m.second - m.first + 1);
    }

    REGISTER_SOLUTION(2025, 5, 1, Solve_1);
    REGISTER_SOLUTION(2025, 5, 2, Solve_2_Multipass, "multipass merge");
    REGISTER_SOLUTION(2025, 5, 2, Solve_2_Sorted, "sort merge");
}
