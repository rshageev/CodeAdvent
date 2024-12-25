#include "Runner.h"

import std;
import utils;

namespace
{
    using Keypad = Array2D<char>;
    static const Keypad KeypadN = Array2DFromString("789\n456\n123\n.0A");
    static const Keypad KeypadD = Array2DFromString(".^A\n<v>");


    // Find all shortest sequences we need to press on a digital keypad in order for the robot
    // controlled by us to press `tgt_key` from the initial position at `start_key`
    std::vector<std::string> FindSequences(const Array2D<char>& tgt_keypad, char start_key, char tgt_key)
    {
        std::queue<std::pair<Point, std::string>> q;
        Array2D<bool> visited(tgt_keypad.Area(), false);
        std::vector<std::string> result;

        Point start_pos = FindInArray2D(tgt_keypad, start_key);
        q.emplace(start_pos, "");

        while (!q.empty())
        {
            auto [pos, presses] = q.front();
            q.pop();

            visited[pos] = true;

            if (tgt_keypad[pos] == tgt_key)
            {
                result.push_back(presses + 'A');
            }
            else
            {
                for (auto [dir, off] : Neighbours4) {
                    Point np = pos + off;
                    if (tgt_keypad.Contains(np) && tgt_keypad[np] != '.' && !visited[np]) {
                        q.emplace(np, presses + DirToChar(dir));
                    }
                }
            }
        }

        return result;
    }

    using CacheKey = std::pair<std::string, int>;
    using Cache = std::map<CacheKey, int64>;

    int64 MinPresses(char from, char to, int robot_idx, int max_robots, Cache& cache);

    // seq - what we need to input on our keypad
    // robot_idx - our index (0 - human)
    // max_robots - robot controlling the door
    // result - how much presses the human in the end of the chain has to do
    int64 MinPresses(std::string_view seq, int robot_idx, int max_robots, Cache& cache)
    {
        if (robot_idx == 0) {
            return std::ssize(seq);
        }

        CacheKey ckey = { std::string(seq), robot_idx };
        auto [cache_itr, inserted] = cache.try_emplace(ckey);
        if (!inserted) {
            return cache_itr->second;
        }

        int64 result = 0;
        char from = 'A';
        for (char to : seq) {
            result += MinPresses(from, to, robot_idx, max_robots, cache);
            from = to;
        }

        cache_itr->second = result;

        return result;
    }

    // from,to - keys we need to move from and press on a robot we are controlling
    // robot_idx - our index
    // max_robots - robot controlling the door
    // max_robots+1 - the door
    // result - how much presses the human in the end of the chain has to do
    int64 MinPresses(char from, char to, int robot_idx, int max_robots, Cache& cache)
    {
        const Keypad& kpad = (robot_idx == max_robots) ? KeypadN : KeypadD;

        // check all sequences that needs to be input on OUR keyboard to move the TARGET keyboard from->to
        int64 res = std::numeric_limits<int64>::max();
        for (const auto& seq : FindSequences(kpad, from, to)) {
            res = std::min(res, MinPresses(seq, robot_idx - 1, max_robots, cache));
        }
        return res;
    }

    int64 CodeComplexity(std::string_view code, int robots, Cache& cache)
    {
        const int64 len = MinPresses(code, robots, robots, cache);
        const int64 num = Read<int64>(code);
        return len * num;
    }

    int64 Solve(const std::filesystem::path& input, int robots)
    {
        Cache cache;
        int64 res = 0;
        for (const auto& line : ReadLines(input)) {
            res += CodeComplexity(line, robots, cache);
        }
        return res;
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        return Solve(input, 3);
    }

    int64 Solve_2(const std::filesystem::path& input)
    {
        return Solve(input, 26);
    }

    REGISTER_SOLUTION(2024, 21, 1, Solve_1);
    REGISTER_SOLUTION(2024, 21, 2, Solve_2);
}