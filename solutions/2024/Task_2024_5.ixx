export module AoC_2024.Day5;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

auto LoadData(const std::filesystem::path& input)
{
    std::vector<std::pair<int, int>> ordering;
    std::vector<std::vector<int>> tasks;

    bool parse_rules = true;
    for (std::string_view line : ReadLines(input))
    {
        if (line.empty()) {
            parse_rules = false;
        } else if (parse_rules) {
            int v1 = Read<int>(line);
            Skip(line, "|");
            int v2 = Read<int>(line);
            ordering.emplace_back(v1, v2);
        } else {
            std::vector<int> pages;
            do {
                pages.push_back(Read<int>(line));
            } while (Skip(line, ","));
            tasks.push_back(std::move(pages));
        }
    }
    return std::pair(ordering, tasks);
}

bool IsBefore(int v1, int v2, const auto& ordering) {
    return stdr::find(ordering, std::pair(v1, v2)) != ordering.end();
}

bool IsSorted(const std::vector<int>& task, const auto& ordering)
{
    for (int i = 0; i < task.size(); ++i) {
        for (int j = i + 1; j < task.size(); ++j) {
            if (IsBefore(task[j], task[i], ordering)) {
                return false;
            }
        }
    }
    return true;
}

void Sort(std::vector<int>& task, const auto& ordering)
{
    while (!IsSorted(task, ordering)) {
        for (int i = 0; i < task.size(); ++i) {
            for (int j = i + 1; j < task.size(); ++j) {
                if (IsBefore(task[j], task[i], ordering)) {
                    std::swap(task[i], task[j]);
                }
            }
        }
    } 
}

int Solve_1(const std::filesystem::path& input)
{
    auto [ordering, tasks] = LoadData(input);

    int result = 0;
    for (const auto& task : tasks) {
        if (IsSorted(task, ordering)) {
            result += task[task.size() / 2];
        }
    }
    return result;
}

int Solve_2(const std::filesystem::path& input)
{
    auto [ordering, tasks] = LoadData(input);

    int result = 0;
    for (auto& task : tasks) {
        if (!IsSorted(task, ordering)) {
            Sort(task, ordering);
            result += task[task.size() / 2];
        }
    }
    return result;
}