#include "Runner.h"

import std;
import utils;

namespace
{
    constexpr std::uint64_t CalcDistance(std::uint64_t race_time, std::uint64_t hold_time)
    {
        return hold_time * (race_time - hold_time);
    }

    struct Race
    {
        std::uint64_t time;
        std::uint64_t record;
    };

    std::uint64_t CountWaysToWin(Race race)
    {
        std::uint64_t count = 0;
        for (std::uint64_t hold_time = 0; hold_time <= race.time; ++hold_time)
        {
            const auto distance = CalcDistance(race.time, hold_time);
            if (distance > race.record) {
                ++count;
            }
        }
        return count;
    }

    std::vector<Race> LoadData_1(const std::filesystem::path& input)
    {
        const auto lines = ReadLines(input);

        std::vector<Race> races;
        std::regex regex{ R"(\d+)" };

        auto times = std::ranges::subrange(std::sregex_iterator(lines[0].begin(), lines[0].end(), regex), std::sregex_iterator());
        auto dist = std::ranges::subrange(std::sregex_iterator(lines[1].begin(), lines[1].end(), regex), std::sregex_iterator());
        for (auto [tm, dm] : stdv::zip(times, dist)) {
            races.emplace_back(to_uint64(tm.str()), to_uint64(dm.str()));
        }
        return races;
    }

    Race LoadData_2(const std::filesystem::path& input)
    {
        const auto lines = ReadLines(input);

        std::regex regex{ R"(\d+)" };

        std::string time_str;
        std::string dist_str;
        auto times = std::ranges::subrange(std::sregex_iterator(lines[0].begin(), lines[0].end(), regex), std::sregex_iterator());
        auto dist = std::ranges::subrange(std::sregex_iterator(lines[1].begin(), lines[1].end(), regex), std::sregex_iterator());
        for (auto [tm, dm] : stdv::zip(times, dist)) {
            time_str.append(tm.str());
            dist_str.append(dm.str());
        }
        return Race{ to_uint64(time_str), to_uint64(dist_str) };
    }

    std::uint64_t Solve_1(const std::filesystem::path& input)
    {
        std::uint64_t result = 1;
        for (const auto& race : LoadData_1(input))
        {
            const auto win_times = CountWaysToWin(race);
            result *= win_times;
        }
        return result;
    }

    std::uint64_t Solve_2(const std::filesystem::path& input)
    {
        return CountWaysToWin(LoadData_2(input));
    }

    REGISTER_SOLUTION(2023, 6, 1, Solve_1);
    REGISTER_SOLUTION(2023, 6, 2, Solve_2);
}