export module AoC_2023.Day6;

import std;
import utils;

export std::uint64_t Solve_1();
export std::uint64_t Solve_2();

module : private;

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

std::uint64_t Solve_1()
{
    constexpr Race test_races[] = { { 7, 9 }, { 15, 40 }, { 30, 200 } };
    constexpr Race races[] = { { 38, 234 }, { 67, 1027 }, { 76, 1157 }, { 73, 1236 } };

    std::uint64_t result = 1;
    for (const auto& race : races)
    {
        const auto win_times = CountWaysToWin(race);
        std::cout << win_times << '\n';
        result *= win_times;
    }
    return result;
}

std::uint64_t Solve_2()
{
    constexpr Race test_race = { 71530, 940200 };
    constexpr Race race = { 38677673, 234102711571236 };

    return CountWaysToWin(race);
}