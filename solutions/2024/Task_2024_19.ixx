module;
#include <scn/scan.h>

export module AoC_2024.Day19;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

namespace
{
    auto LoadData(const std::filesystem::path& input)
    {
        auto lines = ReadLines(input);

        std::vector<std::string> parts;
        std::string_view str = lines[0];
        do { parts.emplace_back(ReadWord(str)); } while (Skip(str, ", "));

        lines.erase(lines.begin(), lines.begin() + 2);

        return std::pair(parts, lines);
    }

    using Cache = std::unordered_map<std::string_view, int64>;

    int64 CountDesigns(std::string_view design, std::span<const std::string> parts, Cache& cache) {
        if (design.empty()) {
            return 1;
        }
        if (auto itr = cache.find(design); itr != cache.end()) {
            return itr->second;
        }
        int64 count = 0;
        for (auto p : parts) {
            if (design.starts_with(p)) {
                count += CountDesigns(design.substr(p.size()), parts, cache);
            }
        }

        cache[design] = count;
        return count;
    };
}

int64 Solve_1(const std::filesystem::path& input)
{
    auto [parts, designs] = LoadData(input);

    int64 res = 0;
    Cache cache;
    for (const auto& design : designs) {
        res += std::min(1ll, CountDesigns(design, parts, cache));
    }
    return res;
}

int64 Solve_2(const std::filesystem::path& input)
{
    auto [parts, designs] = LoadData(input);

    int64 res = 0;
    Cache cache;
    for (const auto& design : designs) {
        res += CountDesigns(design, parts, cache);
    }
    return res;
}