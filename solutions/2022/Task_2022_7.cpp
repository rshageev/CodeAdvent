#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    struct Directory
    {       
        std::vector<Directory> dirs;
        int size = 0;
    };

    int CalcSize(Directory& dir)
    {
        return (dir.size += Sum(dir.dirs | stdv::transform(CalcSize)));
    }

    Directory LoadInput(const std::filesystem::path& input)
    {
        Directory root;

        std::vector<Directory*> current = { &root };

        for (std::string_view line : ReadLines(input) | stdv::drop(1))
        {
            if (line.starts_with("$ cd"))
            {
                if (line.substr(5) == "..") {
                    current.pop_back();
                } else {
                    auto& new_dir = current.back()->dirs.emplace_back();
                    current.push_back(&new_dir);
                } 
            }
            else if (is_digit(line[0]))
            {
                current.back()->size += Read<int>(line);
            }
        }

        CalcSize(root);

        return root;
    }

    int Solve1_Helper(const Directory& dir)
    {
        int res = (dir.size < 100'000) ? dir.size : 0;
        for (const auto& subdir : dir.dirs) {
            res += Solve1_Helper(subdir);
        }
        return res;
    }

    int Solve2_Helper(const Directory& dir, int req_size)
    {
        int res = (dir.size >= req_size) ? dir.size : std::numeric_limits<int>::max();
        for (const auto& subdir : dir.dirs) {
            res = std::min(res, Solve2_Helper(subdir, req_size));
        }
        return res;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        auto root = LoadInput(input);
        return Solve1_Helper(root);
    }

    int Solve_2(const std::filesystem::path& input)
    {
        auto root = LoadInput(input);
        return Solve2_Helper(root, root.size - 40'000'000);
    }

    REGISTER_SOLUTION(2022, 7, 1, Solve_1);
    REGISTER_SOLUTION(2022, 7, 2, Solve_2);
}
