#include "Runner.h"

import std;
import utils;

namespace
{
    using IdxType = uint64;

    struct Range {
        IdxType start;
        IdxType size;
    };

    struct RangePair {
        IdxType out;
        IdxType in;
        IdxType size;
    };

    class RangesMap
    {
    public:
        void Add(RangePair rng)
        {
            ranges.push_back(rng);
        }

        IdxType operator[] (IdxType v) const
        {
            auto itr = FindRangePair(v);
            if (itr != stdr::end(ranges)) {
                return itr->out + (v - itr->in);
            }
            return v;
        }

        void MapRange(std::vector<Range>& out, Range r) const
        {
            auto end_itr = stdr::end(ranges);

            {
                auto itr = FindRangePair(r.start);
                while (itr != end_itr && r.size > 0)
                {
                    auto new_start = itr->out + (r.start - itr->in);
                    auto new_size = std::min(r.size, itr->out + itr->size - new_start);
                    out.emplace_back(new_start, new_size);

                    r.start += new_size;
                    r.size -= new_size;
                    itr = FindRangePair(r.start);
                }
            }
            {
                auto itr = FindRangePair(r.start + r.size - 1);
                while (itr != end_itr && r.size > 0)
                {
                    auto new_end = itr->out + (r.start + r.size - 1 - itr->in);
                    auto new_size = std::min(r.size, new_end - itr->out + 1);
                    out.emplace_back(new_end + 1 - new_size, new_size);

                    r.size -= new_size;
                    itr = FindRangePair(r.start + r.size - 1);
                }
            }

            if (r.size > 0) {
                out.push_back(r);
            }
        }
    private:
        std::vector<RangePair> ranges;

        auto FindRangePair(IdxType v) const {
            return stdr::find_if(ranges, [v](const auto& r) {
                return v >= r.in && v < (r.in + r.size);
                });
        }
    };

    struct Almanac
    {
        std::vector<IdxType> seeds;
        std::vector<RangesMap> maps;
    };

    RangePair ReadRangeLine(std::string_view str)
    {
        RangePair r;
        r.out = Read<IdxType>(str);
        Skip(str, " ");
        r.in = Read<IdxType>(str);
        Skip(str, " ");
        r.size = Read<IdxType>(str);
        return r;
    }

    Almanac LoadAlmanac(const std::filesystem::path& input)
    {
        Almanac almanac;

        const auto lines = ReadLines(input);

        // read seeds
        std::string_view seeds_line = lines[0];
        Skip(seeds_line, "seeds: ");
        while (!seeds_line.empty()) {
            const auto seed = Read<IdxType>(seeds_line);
            Skip(seeds_line, " ");
            almanac.seeds.push_back(seed);
        }

        // read maps
        for (const auto& line : lines | stdv::drop(2))
        {
            if (is_digit(line[0])) {
                almanac.maps.back().Add(ReadRangeLine(line));
            }
            else {
                almanac.maps.emplace_back();
            }
        }

        return almanac;
    }

    IdxType SeedToLocation(const Almanac& almanac, IdxType seed)
    {
        auto value = seed;
        for (const auto& map : almanac.maps) {
            value = map[value];
        }
        return value;
    }

    IdxType Solve_1(const std::filesystem::path& input)
    {
        const auto almanac = LoadAlmanac(input);

        IdxType min_location = std::numeric_limits<IdxType>::max();
        for (auto seed : almanac.seeds)
        {
            const auto loc = SeedToLocation(almanac, seed);
            min_location = std::min(loc, min_location);
        }
        return min_location;
    }


    std::vector<Range> SeedToLocationRanges(std::vector<Range> seeds, const Almanac& almanac)
    {
        std::vector<Range> in = std::move(seeds);
        std::vector<Range> out;

        for (const auto& map : almanac.maps)
        {
            out.clear();
            for (const auto& in_range : in) {
                map.MapRange(out, in_range);
            }
            in = std::move(out);
        }

        return in;
    }

    std::vector<Range> SeedsToRanges(const std::vector<IdxType>& seeds)
    {
        std::vector<Range> seeds_r;
        for (auto r : seeds | stdv::chunk(2))
        {
            auto itr = stdr::begin(r);
            const IdxType start = *itr++;
            const IdxType size = *itr;
            seeds_r.emplace_back(start, size);
        }
        return seeds_r;
    }

    IdxType Solve_2(const std::filesystem::path& input)
    {
        const auto almanac = LoadAlmanac(input);

        const auto seeds = SeedsToRanges(almanac.seeds);

        const auto locations = SeedToLocationRanges(seeds, almanac);

        IdxType min_location = std::numeric_limits<IdxType>::max();
        for (auto [start, size] : locations) {
            min_location = std::min(min_location, start);
        }

        return min_location;
    }

    REGISTER_SOLUTION(2023, 5, 1, Solve_1);
    REGISTER_SOLUTION(2023, 5, 2, Solve_2);
}