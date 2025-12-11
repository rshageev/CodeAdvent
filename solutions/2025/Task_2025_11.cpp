#include "Runner.h"

import std;
import utils;

namespace
{
    using Devices = std::unordered_map<std::string, std::vector<std::string>>;

    auto ReadData(const std::filesystem::path& input)
    {
        Devices devices;
        for (std::string_view line : ReadLines(input)) {
            auto name = std::string(line.substr(0, 3));
            line.remove_prefix(4);

            std::vector<std::string> outputs;
            while (!line.empty()) {
                auto out = line.substr(1, 3);
                line.remove_prefix(4);
                outputs.emplace_back(std::string(out));
            }

            devices.emplace(name, std::move(outputs));
        }
        return devices;
    }

    /**** Part 1 *************************************************/

    uint64 SolveDFS1(const Devices& devices, std::string from)
    {
        if (from == "out")
            return 1;
        uint64 count = 0;
        for (const auto& to : devices.at(from)) {
            count += SolveDFS1(devices, to);
        }
        return count;
    }

    uint64 Solve_1_DFS(const std::filesystem::path& input)
    {
        const auto devices = ReadData(input);
        return SolveDFS1(devices, "you");
    }

    uint64 Solve_1_BFS(const std::filesystem::path& input)
    {
        const auto devices = ReadData(input);

        uint64 count = 0;

        std::queue<std::string> q;
        q.push("you");

        while (!q.empty()) {
            auto from = q.front();
            q.pop();

            for (const auto& to : devices.at(from)) {
                if (to == "out") {
                    ++count;
                } else {
                    q.push(to);
                }
            }
        }

        return count;
    }

    /**** Part 2 *************************************************/

    using CacheKey = std::tuple<std::string, bool, bool>;
    using Cache = std::map<CacheKey, uint64>;
    
    uint64 SolveDFS_2(Cache& cache, const Devices& devices, const std::string& from, bool dac, bool fft)
    {
        if (from == "out") {
            return (dac && fft) ? 1 : 0;
        }

        CacheKey cache_key{ from, dac, fft };

        auto itr = cache.find(cache_key);
        if (itr != cache.end()) {
            return itr->second;
        }

        bool new_dac = dac || (from == "dac");
        bool new_fft = fft || (from == "fft");

        uint64 count = 0;
        for (const auto& to : devices.at(from)) {
            count += SolveDFS_2(cache, devices, to, new_dac, new_fft);
        }

        cache.emplace(cache_key, count);
        return count;
    }

    uint64 Solve_2(const std::filesystem::path& input)
    {
        const auto devices = ReadData(input);

        Cache cache;
        return SolveDFS_2(cache, devices, "svr", false, false);
    }


    REGISTER_SOLUTION(2025, 11, 1, Solve_1_DFS, "dfs");
    REGISTER_SOLUTION(2025, 11, 1, Solve_1_BFS, "bfs");
    REGISTER_SOLUTION(2025, 11, 2, Solve_2);
}
