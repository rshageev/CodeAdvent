#include "Runner.h"

import std;
import utils;
import md5;

namespace
{
    uint32 SolveThread(std::string_view key, std::string_view req_prefix, uint32 start, uint32 stride, std::atomic<uint32>& result)
    {
        uint32 i = start;
        uint32 res = result.load();
        while (i < res)
        {
            std::string str = std::format("{}{}", key, i);
            std::string hash = CalcMD5(str);

            res = result.load();
            if (hash.starts_with(req_prefix)) {
                uint32 new_res = std::min(i, res);
                while (!result.compare_exchange_weak(res, new_res)) {
                    new_res = std::min(i, res);
                }
            }
            i += stride;
        }
        return 0;
    }

    uint32 Solve(std::string_view req_prefix, const std::filesystem::path& input)
    {
        const auto key = ReadText(input);

        const int Threads = std::thread::hardware_concurrency();
        
        std::atomic<uint32> result = std::numeric_limits<uint32>::max();

        std::vector<std::jthread> threads;
        for (int i = 0; i < Threads; ++i) {
            threads.emplace_back(std::bind_front(SolveThread, key, req_prefix, i, Threads, std::ref(result)));
        }
        threads.clear();

        return result.load();
    }

    REGISTER_SOLUTION(2015, 4, 1, std::bind_front(Solve, "00000"));
    REGISTER_SOLUTION(2015, 4, 2, std::bind_front(Solve, "000000"));
}