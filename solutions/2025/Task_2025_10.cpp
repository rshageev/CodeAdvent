#include "Runner.h"

import std;
import utils;

namespace
{
    struct Machine {
        uint16 req_lights = 0;
        std::vector<uint16> buttons;
        std::vector<int> req_joltage;
    };

    Machine ParseLine(std::string_view line)
    {
        Machine m;

        auto parts = Split(line, " ");

        for (auto& part : parts) {
            part.remove_prefix(1);
            part.remove_suffix(1);
        }

        for (auto ch : parts[0] | stdv::reverse) {
            m.req_lights = (m.req_lights << 1) + ((ch == '#') ? 1 : 0);
        }

        for (size_t i = 1; i < parts.size() - 1; ++i) {
            uint16 btn_mask = 0;
            for (auto str : Split(parts[i], ",")) {
                btn_mask = btn_mask | (1u << to_int(str));
            }
            m.buttons.push_back(btn_mask);
        }

        for (auto str : Split(parts.back(), ",")) {
            m.req_joltage.push_back(to_int(str));
        }

        return m;
    }

    auto ReadData(const std::filesystem::path& input)
    {
        return ReadLines(input) | stdv::transform(ParseLine) | stdr::to<std::vector>();
    }

    uint16 Configure(const Machine& machine)
    {
        std::queue<std::pair<uint16, uint16>> q;
        q.emplace(0, 0);

        std::unordered_set<uint16> visited;
        visited.insert(0);

        while (!q.empty())
        {
            auto [lights, count] = q.front();
            q.pop();

            for (auto btn_mask : machine.buttons) {
                uint16 new_lights = lights ^ btn_mask;
                if (new_lights == machine.req_lights) {
                    return count + 1;
                }
                if (visited.insert(new_lights).second) {
                    q.emplace(new_lights, count + 1);
                }
            }
        }

        return 0;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        uint32 presses = 0;
        for (const auto& m : ReadData(input)) {
            presses += Configure(m);
        }
        return presses;
    }

    REGISTER_SOLUTION(2025, 10, 1, Solve_1);
}
