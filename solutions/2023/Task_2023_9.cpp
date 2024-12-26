#include "Runner.h"

import std;
import utils;

namespace
{
    using History = std::vector<std::vector<int>>;

    History ParseLine(std::string_view line)
    {
        History history;
        auto& values = history.emplace_back();

        while (!line.empty()) {
            values.push_back(Read<int>(line));
            Skip(line, " ");
        }

        return history;
    }

    std::vector<int> CalcDifference(std::span<const int> values)
    {
        return values | stdv::pairwise
            | stdv::transform(apply_to_pair_rev<std::minus<>>{})
            | stdr::to<std::vector>();
    }

    bool AllZeroes(std::span<const int> values)
    {
        return stdr::all_of(values, equal<0>);
    }

    void CalcDifferences(History& history)
    {
        while (!AllZeroes(history.back())) {
            auto diff = CalcDifference(history.back());
            history.push_back(std::move(diff));
        }
    }

    int Extrapolate(History& history)
    {
        history.back().push_back(0);

        for (auto i = std::ssize(history) - 2; i >= 0; --i) {
            auto val = history[i].back() + history[i + 1].back();
            history[i].push_back(val);
        }

        return history[0].back();
    }

    int ExtrapolatePrev(History& history)
    {
        history.back().insert(history.back().begin(), 0);

        for (auto i = std::ssize(history) - 2; i >= 0; --i) {
            auto val = history[i].front() - history[i + 1].front();
            history[i].insert(history[i].begin(), val);
        }

        return history[0].front();
    }

    int Solve_1(const std::filesystem::path& input)
    {
        int sum = 0;

        for (const auto& line : ReadLines(input))
        {
            auto history = ParseLine(line);
            CalcDifferences(history);
            sum += Extrapolate(history);
        }
        return sum;
    }

    int Solve_2(const std::filesystem::path& input)
    {
        int sum = 0;

        for (const auto& line : ReadLines(input))
        {
            auto history = ParseLine(line);
            CalcDifferences(history);
            sum += ExtrapolatePrev(history);
        }
        return sum;
    }

    REGISTER_SOLUTION(2023, 9, 1, Solve_1);
    REGISTER_SOLUTION(2023, 9, 2, Solve_2);
}