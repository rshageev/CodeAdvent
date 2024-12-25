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

    REGISTER_SOLUTION(2024, 24, 1, Solve_1);
}
