#include "Runner.h"

import std;
import utils;

namespace
{
    using OpFunc = void(*)(int&);

    void SetOn(int& v) { v = 1; }
    void SetOff(int& v) { v = 0; }
    void Toggle(int& v) { v = 1 - v; }

    void Inc(int& v) { v += 1; }
    void Dec(int& v) { v = std::max(0, v - 1); }
    void Inc2(int& v) { v += 2; }

    auto GenLightsMap(const std::filesystem::path& input, std::array<OpFunc,3> op_set)
    {
        const std::regex regex(R"((turn on|turn off|toggle) (\d+),(\d+) through (\d+),(\d+))");

        Array2D<int> map(1000, 1000, 0);

        for (const auto line : ReadLines(input))
        {
            std::smatch m;
            if (std::regex_match(line, m, regex))
            {
                OpFunc op = nullptr;
                // parse operation
                std::string op_str = m[1].str();
                if (op_str == "turn on") op = op_set[0];
                if (op_str == "turn off") op = op_set[1];
                if (op_str == "toggle") op = op_set[2];

                // parse rect
                int x1 = to_int(m[2].str());
                int y1 = to_int(m[3].str());
                int x2 = to_int(m[4].str());
                int y2 = to_int(m[5].str());
                Rect rect = { x1, y1, x2 - x1 + 1, y2 - y1 + 1 };

                // apply operation to the map
                for (Point pt : to_cell_coords(rect)) {
                    op(map[pt]);
                }
            }
        }
        return map;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        auto map = GenLightsMap(input, { SetOn, SetOff, Toggle });
        return stdr::count(map, 1);
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        auto map = GenLightsMap(input, { Inc, Dec, Inc2 });
        return Sum(map);
    }

    REGISTER_SOLUTION(2015, 6, 1, Solve_1);
    REGISTER_SOLUTION(2015, 6, 2, Solve_2);
}
