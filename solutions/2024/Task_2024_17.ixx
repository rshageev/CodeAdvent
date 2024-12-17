export module AoC_2024.Day17;

import std;
import utils;

export std::string Solve_1(const std::filesystem::path& input);
export uint64 Solve_2(const std::filesystem::path& input);

module : private;

namespace
{
    struct Registers {
        uint64 a, b, c;
    };

    auto LoadData(const std::filesystem::path& input)
    {
        const auto lines = ReadLines(input);

        Registers reg;
        Parse{ lines[0] }.Skip("Register A: ").Read<uint64>(reg.a);
        Parse{ lines[1] }.Skip("Register B: ").Read<uint64>(reg.b);
        Parse{ lines[2] }.Skip("Register C: ").Read<uint64>(reg.c);

        std::vector<uint8> program;
        std::string_view str = lines[4];
        Skip(str, "Program: ");
        do { program.push_back(Read<uint8>(str)); } while (Skip(str, ","));

        return std::pair(reg, program);
    }

    uint64 OpValue(uint64 arg, const Registers& reg)
    {
        if (arg == 4) return reg.a;
        if (arg == 5) return reg.b;
        if (arg == 6) return reg.c;
        return arg;
    }

    std::vector<uint8> Run(std::span<const uint8> ops, Registers reg)
    {
        int ip = 0;
        int max_ip = static_cast<int>(ops.size() - 1);
        std::vector<uint8> out;

        while (ip < max_ip)
        {
            uint64 op = ops[ip];
            uint64 arg = OpValue(ops[ip + 1], reg);
            ip += 2;

            /*
            0, 3 | adv 3 | A = A >> 3
            5, 4 | out A | out A % 8
            3, 0 | jnz 0 | loop if A != 0
            */

            switch (op)
            {
            case 0: // adv
                reg.a = reg.a >> arg; break;
            case 1: // bxl
                reg.b = reg.b ^ arg;  break;
            case 2: // bst
                reg.b = arg % 8; break;
            case 3: // jnz
                if (reg.a != 0) { ip = arg; } break;
            case 4: // bxc
                reg.b = reg.b ^ reg.c; break;
            case 5: // out
                out.push_back(arg % 8); break;
            case 6: // bdv
                reg.b = reg.a >> arg; break;
            case 7: // cdv
                reg.c = reg.a >> arg; break;
            }
        }

        return out;
    }

    uint64 FindA(uint64 base_a, std::span<const uint8> ops)
    {
        // appending 3 bits more to the a, if they cause numbers to match, until we match them all
        for (uint8 v = 0; v < 8; ++v) {
            uint64 a = (base_a << 3) + v;
            auto out = Run(ops, { a, 0, 0 });
            if (stdr::ends_with(ops, out)) {
                if (out.size() == ops.size())
                    return a;
                if (a == 0) // adding 0 bits to 0 changes nothing, we've already checked this one
                    continue;
                if (a = FindA(a, ops); a > 0)
                    return a;
            }
        }
        return 0;
    }
}

std::string Solve_1(const std::filesystem::path& input)
{
    auto [reg, ops] = LoadData(input);

    auto out = Run(ops, reg);

    std::string res;
    for (auto v : out) {
        res.append(std::to_string(v));
        res.push_back(',');
    }
    if (!res.empty() && res.back() == ',') res.pop_back();

    return res;
}

uint64 Solve_2(const std::filesystem::path& input)
{
    const auto [_, ops] = LoadData(input);

    return FindA(0, ops);
}