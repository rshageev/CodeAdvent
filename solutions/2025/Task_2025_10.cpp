#include "Runner.h"

import std;
import utils;
import Z3;

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

    /***************************************************************/

    uint32 Configure1_BFS(const Machine& machine)
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

    /***************************************************************/

    uint32 Configure2_Z3(const Machine& machine)
    {
        z3::context ctx;
        z3::optimize opt(ctx);

        z3::expr_vector btnPresses(ctx);
        for (size_t b = 0; b < machine.buttons.size(); ++b) {
            auto btnName = std::format("button_{}", b);
            btnPresses.push_back(ctx.int_const(btnName.c_str()));
            opt.add(btnPresses.back() >= 0);
        }

        for (size_t j = 0; j < machine.req_joltage.size(); ++j) {
            z3::expr_vector terms(ctx);
            for (size_t b = 0; b < machine.buttons.size(); ++b) {
                if (machine.buttons[b] & (1u << j)) {
                    terms.push_back(btnPresses[b]);
                }
            }
            z3::expr joltage = z3::sum(terms);
            z3::expr reqJoltage = ctx.int_val(machine.req_joltage[j]);
            opt.add(joltage == reqJoltage);
        }

        opt.minimize(z3::sum(btnPresses));
        opt.check();

        z3::model m = opt.get_model();
        uint32 sum = 0;
        for (const auto& btnVar : btnPresses) {
			sum += m.eval(btnVar).get_numeral_uint();
        }
        return sum;
    }

    /***************************************************************/

	using ConfigureFunc = uint32(*)(const Machine&);
    auto Solve(ConfigureFunc configure, const std::filesystem::path& input)
    {
        return Sum(ReadData(input) | stdv::transform(configure));
    }

    REGISTER_SOLUTION(2025, 10, 1, std::bind_front(Solve, Configure1_BFS));
    REGISTER_SOLUTION(2025, 10, 2, std::bind_front(Solve, Configure2_Z3));
}
