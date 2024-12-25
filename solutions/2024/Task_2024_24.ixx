module;
#include <scn/scan.h>

export module AoC_2024.Day24;

import std;
import utils;

export uint64 Solve_1(const std::filesystem::path& input);
export uint64 Solve_2(const std::filesystem::path& input);

module : private;

namespace
{
    int RegBit(std::string_view name)
    {
        return Read<int>(name.substr(1), -1);
    }

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

    void WriteValue(uint64 x, char reg, Values& values)
    {
        for (auto& p : values) {
            if (p.first[0] == reg) {
                uint32 bit = (x % 2) ? 1 : 0;
                p.second = bit;
                x = x >> 1;
            }
        }
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

    using RegMap = std::map<std::string, std::string>;

    struct Rule
    {
        char r1;
        std::string op;
        char r2;
        char rout;
        int out_inc = 0;

        std::optional<std::pair<std::string, std::string>> Verify(const Gate& g) const
        {
            if (g.op != op || g.r1[0] != r1 || g.r2[0] != r2) {
                return {};
            }

            int bit1 = RegBit(g.r1);
            int bit2 = RegBit(g.r2);
            int bit3 = RegBit(g.rout);

            if (bit1 != bit2) {
                std::cout << "Circuit is not a valid adder\n";
            }

            std::string out = std::format("{}{:02}", rout, bit1 + out_inc);
            if (g.rout != out)
            {
                return { { g.rout, out } };
            }

            return {};;
        }

        std::optional<std::string> DeduceOut(const Gate& g) const
        {
            if (RegBit(g.rout) >= 0) {
                return {}; // do not deduce already allocated names
            }
            if (g.op != op || g.r1[0] != r1 || g.r2[0] != r2) {
                return {};
            }

            int b1 = RegBit(g.r1);
            int b2 = RegBit(g.r2);
            if (b1 == b2) {
                std::string out = std::format("{}{:02}", rout, b1 + out_inc);
                return { out };
            }

            return {};
        }

        std::optional<std::string> DeduceR1(const Gate& g) const
        {
            if (RegBit(g.r1) >= 0) {
                return {}; // do not deduce already allocated names
            }
            if (g.op != op || g.r2[0] != r2 || g.rout[0] != rout) {
                return {};
            }

            int b1 = RegBit(g.r1);
            int b2 = RegBit(g.r2);
            int b3 = RegBit(g.rout);

            if (b1 < 0 && (b3 == b2 + out_inc)) {
                // r1 unallocated, r2 and r3 match
                std::string dr1 = std::format("{}{:02}", rout, b2 + out_inc);
                return { dr1 };
            }
            return {};
        }
    };

    bool CmpReg(std::string_view r1, std::string_view r2)
    {
        int b1 = RegBit(r1);
        int b2 = RegBit(r2);

        if (b1 != b2) {
            return b1 < b2;
        } else {
            return r1 < r2;
        }
    }

    void ApplySubs(Gates& gates, const RegMap& subs)
    {
        for (auto& g : gates) {
            if (auto itr = subs.find(g.r1); itr != subs.end()) {
                g.r1 = itr->second;
            }
            if (auto itr = subs.find(g.r2); itr != subs.end()) {
                g.r2 = itr->second;
            }
            if (auto itr = subs.find(g.rout); itr != subs.end()) {
                g.rout = itr->second;
            }
            if (!CmpReg(g.r1, g.r2)) {
                std::swap(g.r1, g.r2);
            }
        }
    }

    void DeduceNames(const Rule& rule, Gates& gates, RegMap& subs)
    {
        for (const auto& g : gates) {
            if (auto deduced_r1 = rule.DeduceR1(g)) {
                subs.emplace(g.r1, *deduced_r1);
            }
            else if (auto deduced_out = rule.DeduceOut(g)) {
                subs.emplace(g.rout, *deduced_out);
            }
        }
        ApplySubs(gates, subs);
    }

    void PrintGates(const Gates& gates)
    {
        for (const auto& g : gates) {
            std::cout << std::format("{} {} {} -> {}\n", g.r1, g.op, g.r2, g.rout);
        }
    }
}

uint64 Solve_1(const std::filesystem::path& input)
{
    auto [values, gates] = LoadInput(input);

    RunCircuit(values, gates);
    
    return ReadValue('z', values);
}

uint64 Solve_2(const std::filesystem::path& input)
{
    auto [_, gates] = LoadInput(input);

    if (gates.size() < 100) {
        // it is a test input
        return 0;
    }

    std::set<std::string> all_reg;
    for (const auto& g : gates) {
        all_reg.insert(g.r1);
        all_reg.insert(g.r2);
        all_reg.insert(g.rout);
    }
    std::cout << std::format("Total registers = {}\n", all_reg.size());

    // FN - carry bit
    const Rule rules[] = {
        // first half adder
        {'x', "XOR", 'y', 'A'},
        {'x', "AND", 'y', 'B'},
        // second half adder
        {'A', "XOR", 'C', 'z'},
        {'C', "XOR", 'A', 'z'},
        {'A', "AND", 'C', 'D'},
        {'C', "AND", 'A', 'D'},
        // carry
        {'B', "OR", 'D', 'C', 1},
        {'D', "OR", 'B', 'C', 1},
    };

    RegMap subs;

    size_t sub_size = 0;
    while (true) {
        for (const auto& rule : rules) {
            DeduceNames(rule, gates, subs);
        }
        if (subs.size() == sub_size)
            break;
        sub_size = subs.size();
    }

    //subs.emplace("z00", "A00");
    //subs.emplace("C01", "B00");
    //ApplySubs(gates, subs);

    PrintGates(gates);

    std::vector<std::pair<std::string, std::string>> errors;

    for (const auto& g : gates) {
        for (const auto& rule : rules) {
            if (auto res = rule.Verify(g)) {
                errors.push_back(*res);
            }
        }
    }

    for (const auto& [v1, v2] : errors) {
        std::cout << std::format("{} must be replaced with {}\n", v1, v2);
    }
    //PrintGates(gates);


    return 0;
}