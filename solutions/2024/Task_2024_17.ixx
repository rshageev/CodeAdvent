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
        uint64 ip = 0;
        uint64 max_ip = ops.size() - 1;
        std::vector<uint8> out;

        while (ip < max_ip)
        {
            uint64 op = ops[ip];
            uint64 arg = OpValue(ops[ip + 1], reg);
            ip += 2;

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
        for (uint64 v = 0; v < 8u; ++v) {
            uint64 a = (base_a << 3) | v;
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

    /* BRUTE FORCE! */

    constexpr uint64 PackOps(std::span<const uint8> ops) {
        uint64 res = 0;
        uint64 mul = 1;
        for (auto op : ops) {
            res = res + op * mul;
            mul *= 8;
        }
        return res;
    }

    static_assert(PackOps(std::to_array<uint8>({ 0b001, 0b010, 0b011, 0b111 })) == 0b111'011'010'001);

    bool Check(uint64 ops, uint64 ax)
    {
        for (int i = 0; i < 16; ++i)
        {
            uint64 bx = (ax % 8) ^ 2;
            bx = bx ^ (ax >> bx);
            bx = bx ^ 3;

            if ((bx % 8) != (ops % 8)) return false;
            
            ops >>= 3;
            ax >>= 3;
        }
        return true;
    }

    void CheckBatch(uint64 ops, uint64 start, uint64 end, uint64 step)
    {
        for (uint64 ax = start; ax <= end; ax += step) {
            if (Check(ops, ax)) {
                std::osyncstream(std::cout) << ax << '\n';
                break;
            }
        }
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

void Solve_BruteForce(const std::filesystem::path& input)
{
    const auto [_, ops] = LoadData(input);

    constexpr uint64 start = 0x0000'00FF'FFFF'FFFF; // at least 46 bit to have output length 16
    constexpr uint64 end = 0x0000'FFFF'FFFF'FFFF; // max 48 bit to have output length 16

    const uint32 jobs = std::thread::hardware_concurrency();
    std::osyncstream(std::cout) << std::format("Starting {} threads\n", jobs);

    Timer timer;
    std::vector<std::jthread> threads;
    const uint64 ops_packed = PackOps(ops);
    for (uint32 i = 0; i < jobs; ++i) {
        threads.emplace_back([=]() { CheckBatch(ops_packed, start + i, end, jobs); });
    }
    threads.clear();

    std::cout << timer.Get();
}

uint64 Solve_2(const std::filesystem::path& input)
{
    auto [_, ops] = LoadData(input);
    return FindA(0, ops);
}