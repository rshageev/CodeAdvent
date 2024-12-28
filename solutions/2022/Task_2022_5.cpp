#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    using Stack = std::vector<char>;

    auto ReadStacks(const std::vector<std::string>& lines)
    {
        std::vector<Stack> stacks((lines.back().size() + 2) / 4);

        for (std::string_view str : lines | stdv::reverse | stdv::drop(1)) {
            auto crates = str | stdv::drop(1) | stdv::stride(4);
            for (auto&& [ch, st] : stdv::zip(crates, stacks)) {
                if (ch != ' ') st.push_back(ch);
            }
        }
        return stacks;
    }

    auto ReadMoves(const std::vector<std::string>& lines)
    {
        std::vector<std::tuple<int, int, int>> moves;
        for (std::string_view line : lines) {
            if (auto res = scn::scan<int, int, int>(line, "move {} from {} to {}")) {
                moves.push_back(res->values());
            }
        }
        return moves;
    }

    auto LoadData(const std::filesystem::path& input)
    {
        const auto sections = ReadLineBlocks(input);
        return std::pair(ReadStacks(sections[0]), ReadMoves(sections[1]));
    }

    void PerformMove(std::span<Stack> stacks, int count, int from, int to)
    {
        auto& src = stacks[from - 1];
        auto& dst = stacks[to - 1];
        for (int i = 0; i < count; ++i) {
            char ch = src.back();
            src.pop_back();
            dst.push_back(ch);
        }
    }

    void PerformMove9001(std::span<Stack> stacks, int count, int from, int to)
    {
        auto& src = stacks[from - 1];
        auto& dst = stacks[to - 1];
        auto start_itr = src.begin() + (src.size() - count);
        std::move(start_itr, src.end(), std::back_inserter(dst));
        src.erase(start_itr, src.end());
    }

    std::string StackTops(std::span<const Stack> stacks) {
        std::string str;
        for (const auto& st : stacks) {
            str.push_back(st.back());
        }
        return str;
    }

    using MoveFunc = void (*)(std::span<Stack>, int, int, int);

    std::string Solve(MoveFunc move, const std::filesystem::path& input)
    {
        auto [stacks, moves] = LoadData(input);
        for (auto [cnt, from, to] : moves) {
            move(stacks, cnt, from, to);
        }
        return StackTops(stacks);
    }

    REGISTER_SOLUTION(2022, 5, 1, std::bind_front(Solve, PerformMove));
    REGISTER_SOLUTION(2022, 5, 2, std::bind_front(Solve, PerformMove9001));
}
