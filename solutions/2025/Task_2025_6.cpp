#include "Runner.h"

import std;
import utils;

namespace
{
    using Problem = std::vector<uint64>;

    uint64 SolveProblem(const Problem& problem, char op)
    {
        if (op == '+') {
            return std::accumulate(problem.begin(), problem.end(), uint64(0), std::plus{});
        } else {
            return std::accumulate(problem.begin(), problem.end(), uint64(1), std::multiplies{});
        }
    }

    auto ParseProblems_1(std::span<std::string> lines)
    {
        std::vector<std::vector<std::string_view>> parts;
        for (const auto& line : lines) {
            parts.push_back(Split(line, " ", true));
            std::erase(parts.back(), "");
        }

        std::vector<Problem> problems(parts[0].size());
        for (auto [idx, prob] : problems | stdv::enumerate) {
            for (auto row : parts) {
                prob.push_back(to_uint64(row[idx]));
            }
        }
        return problems;
    }

    auto ParseProblems_2(std::span<std::string> lines)
    {
        std::vector<Problem> problems;
        problems.emplace_back();
        while (!lines.back().empty()) {
            std::string num_str;
            for (auto& line : lines) {
                if (line.back() != ' ')
                    num_str.push_back(line.back());
                line.pop_back();
            }
            if (num_str.empty()) {
                problems.emplace_back();
            } else {
                problems.back().push_back(to_uint64(num_str));
            }
        }
        std::reverse(problems.begin(), problems.end());
        return problems;
    }

    using ParseFunc = std::vector<Problem>(*)(std::span<std::string>);
    auto Solve(ParseFunc parseProblems, const std::filesystem::path& input)
    {
        auto lines = ReadLines(input);

        std::string ops = lines.back();
        std::erase(ops, ' ');
        lines.pop_back();
        
        const auto problems = parseProblems(lines);

        uint64 sum = 0;
        for (const auto& [p, op] : stdv::zip(problems, ops)) {
            sum += SolveProblem(p, op);
        }
        return sum;
    }

    REGISTER_SOLUTION(2025, 6, 1, std::bind_front(Solve, ParseProblems_1));
    REGISTER_SOLUTION(2025, 6, 2, std::bind_front(Solve, ParseProblems_2));
}
