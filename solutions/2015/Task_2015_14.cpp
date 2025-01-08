#include "Runner.h"

import std;
import utils;

namespace
{
    struct Reindeer
    {
        int speed;
        int fly_time;
        int rest_time;
    };

    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<Reindeer> reindeers;

        const std::regex regex(R"(\w+ can fly (\d+) km\/s for (\d+) seconds, but then must rest for (\d+) seconds.)");
        for (const auto& line : ReadLines(input))
        {
            std::smatch match;
            if (std::regex_match(line, match, regex)) {
                reindeers.emplace_back(to_int(match[1]), to_int(match[2]), to_int(match[3]));
            }
        }
        return reindeers;
    }

    int CalcDistance(int time, const Reindeer& rd)
    {
        const int cycle_time = rd.fly_time + rd.rest_time;
        const int fly_seconds = (time / cycle_time) * rd.fly_time + std::min(rd.fly_time, time % cycle_time);
        return fly_seconds * rd.speed;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        const int time = (input.filename() == "input.txt") ? 2503 : 1000;

        return stdr::max(LoadData(input) | stdv::transform(std::bind_front(CalcDistance, time)));
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        const int time = (input.filename() == "input.txt") ? 2503 : 1000;
        
        const auto reindeers = LoadData(input);
        std::vector<int> scores(reindeers.size(), 0);

        for (int t = 1; t <= time; ++t)
        {
            std::vector<size_t> round_winners;
            int best_dist = 0;
            for (const auto& [idx, rd] : reindeers | stdv::enumerate) {
                int dist = CalcDistance(t, rd);
                if (dist > best_dist) {
                    round_winners.assign(1, idx);
                    best_dist = dist;
                } else if (dist == best_dist) {
                    round_winners.push_back(idx);
                }
            }

            for (size_t widx : round_winners) {
                scores[widx]++;
            }
        }

        return stdr::max(scores);
    }

    REGISTER_SOLUTION(2015, 14, 1, Solve_1);
    REGISTER_SOLUTION(2015, 14, 2, Solve_2);
}
