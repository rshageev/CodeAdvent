#include "Runner.h"

import std;
import utils;

namespace
{

    struct Block {
        int id = -1;
        int count = 0;
    };

    auto LoadData(const std::filesystem::path& input)
    {
        const auto str = ReadText(input);
        std::vector<Block> blocks;
        int id = 0;
        for (auto v : str | stdv::chunk(2)) {
            blocks.emplace_back(id++, to_int(v[0]));
            blocks.emplace_back(-1, to_int(v[1]));
        }
        blocks.pop_back();
        return blocks;
    }

    void OneMove(std::vector<Block>& blocks, size_t free_idx)
    {
        auto& last_b = blocks.back();
        auto& free_b = blocks[free_idx];

        // move
        if (last_b.count == free_b.count) {
            free_b = last_b;
            blocks.pop_back();
        }
        else if (last_b.count > free_b.count) {
            free_b.id = last_b.id;
            last_b.count -= free_b.count;
        }
        else {
            free_b.count -= last_b.count;
            blocks.insert(blocks.begin() + free_idx, last_b);
            blocks.pop_back();
        }

        if (blocks.back().id == -1) blocks.pop_back();
    }

    void Compact(std::vector<Block>& blocks)
    {
        auto free_itr = stdr::find(blocks, -1, &Block::id);
        while (free_itr != blocks.end()) {
            OneMove(blocks, free_itr - blocks.begin());
            free_itr = stdr::find(blocks, -1, &Block::id);
        }
    }


    uint64 Solve_1(const std::filesystem::path& input)
    {
        auto blocks = LoadData(input);
        Compact(blocks);

        uint64 res = 0;
        uint64 pos = 0;
        for (auto [id, sz] : blocks) {
            for (int off = 0; off < sz; ++off) {
                res += id * (pos + off);
            }
            pos += sz;
        }
        return res;
    }

    void Compact2(std::vector<Block>& blocks)
    {
        int max_id = blocks.back().id;
        for (int id = max_id; id > 0; --id)
        {
            auto b_itr = stdr::find(blocks, id, &Block::id);
            auto f_itr = stdr::find_if(blocks, [b_itr](auto& b) { return b.id < 0 && b.count >= b_itr->count; });

            if (f_itr < b_itr)
            {
                auto b = *b_itr;
                b_itr->id = -1;

                if (f_itr->count == b_itr->count) {
                    *f_itr = b;
                }
                else if (f_itr->count > b_itr->count) {
                    f_itr->count -= b.count;
                    blocks.insert(f_itr, b);
                }
            }
        }
    }

    uint64 Solve_2(const std::filesystem::path& input)
    {
        auto blocks = LoadData(input);
        Compact2(blocks);

        uint64 res = 0;
        uint64 pos = 0;
        for (auto [id, sz] : blocks) {
            if (id >= 0) {
                for (int off = 0; off < sz; ++off) {
                    res += id * (pos + off);
                }
            }
            pos += sz;
        }
        return res;
    }

    REGISTER_SOLUTION(2024, 9, 1, Solve_1);
    REGISTER_SOLUTION(2024, 9, 2, Solve_2);
}