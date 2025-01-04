#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    struct Gate
    {
        std::string r1;
        std::string r2;
        std::string rout;
        std::string op;
        bool run = false;
    };

    using Values = std::map<std::string, uint32, std::greater<std::string>>;
    using Gates = std::vector<Gate>;

    auto LoadInput(const std::filesystem::path& input)
    {
        const auto lines = ReadLineBlocks(input);

        Values values;
        for (std::string_view line : lines[0]) {
            auto name = line.substr(0, 3);
            line.remove_prefix(5);
            uint32 val = Read<uint32>(line);
            values.emplace(std::string(name), val);
        }

        Gates gates;
        for (std::string_view line : lines[1]) {
            if (auto res = scn::scan<std::string, std::string, std::string, std::string>(line, "{} {} {} -> {}")) {
                auto [r1, op, r2, rout] = res->values();
                auto& g = gates.emplace_back(r1, r2, rout, op);
            }
        }

        return std::pair(std::move(values), std::move(gates));
    }

    uint64 ReadValue(char reg, const Values& values)
    {
        uint64 res = 0;
        for (const auto& [name, bit] : values) {
            if (name[0] == 'z') {
                res = (res << 1) + bit;
            }
        }
        return res;
    }

    void RunCircuit(Values& values, Gates& gates)
    {
        for (auto& g : gates) {
            g.run = false;
        }

        bool is_running = true;
        while (is_running) {
            is_running = false;
            for (auto& [r1, r2, rout, op, run] : gates) {
                if (!run)
                {
                    auto iv1 = values.find(r1);
                    auto iv2 = values.find(r2);
                    if (iv1 != values.end() && iv2 != values.end())
                    {
                        if (op == "AND") {
                            int res = (iv1->second == 1 && iv2->second == 1) ? 1 : 0;
                            values.emplace(rout, res);
                        }
                        else if (op == "OR") {
                            int res = (iv1->second == 1 || iv2->second == 1) ? 1 : 0;
                            values.emplace(rout, res);
                        }
                        else if (op == "XOR") {
                            int res = (iv1->second != iv2->second) ? 1 : 0;
                            values.emplace(rout, res);
                        }
                        run = true;
                    }
                    is_running = true;
                }
            }
        }
    }

    uint64 Solve_1(const std::filesystem::path& input)
    {
        auto [values, gates] = LoadInput(input);

        RunCircuit(values, gates);

        return ReadValue('z', values);
    }

    std::string Solve_2(const std::filesystem::path& input)
    {
        auto [values, gates] = LoadInput(input);

        std::set<std::string> bad_outputs;

        for (const auto& gate : gates)
        {
            // all output bits come from a xor operation, with the exception of
            // the last one whch is basically a carry bit
            if (gate.rout[0] == 'z' && gate.op != "XOR" && gate.rout != "z45") {
                bad_outputs.insert(gate.rout);
            }

            if (gate.op == "XOR" && gate.rout[0] != 'z')
            {
                // xor operations are only present between x and y bits, or when calculating the resulting z bit
                bool is_xy = (gate.r1[0] == 'x' || gate.r1[0] == 'y')
                    && (gate.r2[0] == 'x' || gate.r2[0] == 'y');

                if (!is_xy)
                {
                    bad_outputs.insert(gate.rout);
                }
                else
                {
                    // the result of the xy xor operation must be consumed by another xor operation, which leads to z bit
                    bool correct = false;
                    for (const auto& gate2 : gates) {
                        if (gate2.op == "XOR" && (gate2.r1 == gate.rout || gate2.r2 == gate.rout)) {
                            correct = true;
                            break;
                        }
                    }
                    if (!correct && !gate.r1.ends_with("00") && !gate.r2.ends_with("00")) {
                        bad_outputs.insert(gate.rout);
                    }
                }
            }

            if (gate.op == "AND")
            {
                bool is_xy = (gate.r1[0] == 'x' || gate.r1[0] == 'y')
                    && (gate.r2[0] == 'x' || gate.r2[0] == 'y');

                if (is_xy) {
                    bool correct = false;
                    for (const auto& gate2 : gates) {
                        if (gate2.op == "OR" && (gate2.r1 == gate.rout || gate2.r2 == gate.rout)) {
                            correct = true;
                            break;
                        }
                    }
                    if (!correct && !gate.r1.ends_with("00") && !gate.r2.ends_with("00")) {
                        bad_outputs.insert(gate.rout);
                    }
                }
            }
        }

        return bad_outputs | stdv::join_with(',') | stdr::to<std::string>();
    }

    REGISTER_SOLUTION(2024, 24, 1, Solve_1);
    REGISTER_SOLUTION(2024, 24, 2, Solve_2);
}
