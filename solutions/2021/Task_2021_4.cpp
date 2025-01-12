#include "Runner.h"

import std;
import utils;

namespace
{
    using Board = std::array<std::pair<uint16, bool>, 25>;

    auto LoadData(const std::filesystem::path& input)
    {
        const auto blocks = ReadLineBlocks(input);

        std::string_view num_line = blocks[0][0];
        std::vector<uint16> numbers;
        do { numbers.push_back(Read<uint16>(num_line)); } while (Skip(num_line, ","));

        std::vector<Board> boards;
        for (const auto& board_lines : blocks | stdv::drop(1))
        {
            int bidx = 0;
            Board& board = boards.emplace_back();
            for (std::string_view line : board_lines) {
                for (int i = 0; i < 5; ++i) {
                    line = line.substr(line.find_first_of("0123456789"));
                    board[bidx++] = { Read<uint16>(line), false };
                }
            }
        }

        return std::pair(std::move(numbers), std::move(boards));
    }

    void MarkNumber(int number, Board& board)
    {
        for (auto&& [n, marked] : board) {
            if (n == number) marked = true;
        }
    }

    bool CheckLine(int start, int stride, const Board& board)
    {
        int idx = start;
        for (int i = 0; i < 5; ++i) {
            if (!board[idx].second) return false;
            idx += stride;
        }
        return true;
    }

    bool CheckWin(const Board& board)
    {
        for (int i = 0; i < 5; ++i) {
            if (CheckLine(i * 5, 1, board)) return true;
        }
        for (int i = 0; i < 5; ++i) {
            if (CheckLine(i, 5, board)) return true;
        }
        return false;
    }

    uint32 SumUnmarked(const Board& board)
    {
        uint32 res = 0;
        for (const auto [n, m] : board) {
            if (!m) {
                res += n;
            }
        }
        return res;
    }

    uint32 Solve_1(const std::filesystem::path& input)
    {
        auto [numbers, boards] = LoadData(input);

        for (uint16 n : numbers)
        {
            for (auto& b : boards) {
                MarkNumber(n, b);
            }

            for (const auto& b : boards) {
                if (CheckWin(b)) {
                    return n * SumUnmarked(b);
                }
            }
        }
        return 0;
    }

    uint32 Solve_2(const std::filesystem::path& input)
    {
        auto [numbers, boards] = LoadData(input);

        for (uint16 n : numbers)
        {
            for (auto& b : boards) {
                MarkNumber(n, b);
            }

            for (auto itr = boards.begin(); itr != boards.end(); ) {
                if (CheckWin(*itr)) {
                    if (boards.size() == 1) {
                        return n * SumUnmarked(*itr);
                    } else {
                        itr = boards.erase(itr);
                    }
                }
                else {
                    ++itr;
                }
            }
        }

        return 0;
    }

    REGISTER_SOLUTION(2021, 4, 1, Solve_1);
    REGISTER_SOLUTION(2021, 4, 2, Solve_2);
}
