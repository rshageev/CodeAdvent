#include "Runner.h"

import std;
import utils;

namespace
{
    enum class Op { And, Or, Not, LShift, RShift, Const };

    using Arg = std::variant<std::monostate, uint16, std::string>;

    struct Gate
    {
        Op op;
        Arg arg0;
        Arg arg1;
        std::string out;
        bool processed = false;
    };

    Arg ArgFromString(std::string_view str)
    {
        return is_digit(str[0]) ? Arg(from_chars_to<uint16>{}(str)) : Arg(std::string(str));
    }

    auto LoadInput(const std::filesystem::path& input)
    {
        const std::regex and_regex(R"((\w+) AND (\w+) -> (\S+))");
        const std::regex or_regex(R"((\w+) OR (\w+) -> (\S+))");
        const std::regex ls_regex(R"((\w+) LSHIFT (\w+) -> (\S+))");
        const std::regex rs_regex(R"((\w+) RSHIFT (\w+) -> (\S+))");
        const std::regex not_regex(R"(NOT (\w+) -> (\S+))");
        const std::regex const_regex(R"((\w+) -> (\S+))");

        std::vector<Gate> gates;

        for (const auto line : ReadLines(input))
        {
            std::smatch match;
            if (std::regex_match(line, match, and_regex))
            {
                gates.emplace_back(Op::And,
                    ArgFromString(match[1].str()),
                    ArgFromString(match[2].str()),
                    match[3].str());
            }
            else if (std::regex_match(line, match, or_regex))
            {
                gates.emplace_back(Op::Or,
                    ArgFromString(match[1].str()),
                    ArgFromString(match[2].str()),
                    match[3].str());
            }
            else if (std::regex_match(line, match, ls_regex))
            {
                gates.emplace_back(Op::LShift,
                    ArgFromString(match[1].str()),
                    ArgFromString(match[2].str()),
                    match[3].str());
            }
            else if (std::regex_match(line, match, rs_regex))
            {
                gates.emplace_back(Op::RShift,
                    ArgFromString(match[1].str()),
                    ArgFromString(match[2].str()),
                    match[3].str());
            }
            else if (std::regex_match(line, match, not_regex))
            {
                gates.emplace_back(Op::Not,
                    ArgFromString(match[1].str()),
                    Arg{},
                    match[2].str());
            }
            else if (std::regex_match(line, match, const_regex))
            {
                gates.emplace_back(Op::Const,
                    ArgFromString(match[1].str()),
                    Arg{},
                    match[2].str());
            }
        }

        return gates;
    }

    auto RunCircuit(std::span<Gate> gates)
    {
        std::unordered_map<std::string, uint16> values;

        while (true)
        {
            // find some gate with known operands
            auto itr = stdr::find_if(gates, [](const Gate& g) {
                return !g.processed && g.arg0.index() != 2 && g.arg1.index() != 2;
                });
            if (itr == gates.end()) {
                break;
            }

            // perform operation
            Gate& g = *itr;
            uint16 res = 0;
            switch (g.op) {
            case Op::And:
                res = std::get<uint16>(g.arg0) & std::get<uint16>(g.arg1);
                break;
            case Op::Or:
                res = std::get<uint16>(g.arg0) | std::get<uint16>(g.arg1);
                break;
            case Op::LShift:
                res = std::get<uint16>(g.arg0) << std::get<uint16>(g.arg1);
                break;
            case Op::RShift:
                res = std::get<uint16>(g.arg0) >> std::get<uint16>(g.arg1);
                break;
            case Op::Not:
                res = ~std::get<uint16>(g.arg0);
                break;
            case Op::Const:
                res = std::get<uint16>(g.arg0);
                break;
            }
            g.processed = true;

            // write operation result into values
            values[g.out] = res;

            // write operation result into the gates
            for (auto& gate : gates)
            {
                if (gate.arg0 == Arg(g.out)) {
                    gate.arg0 = res;
                }
                if (gate.arg1 == Arg(g.out)) {
                    gate.arg1 = res;
                }
            }
        }

        return values;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        auto gates = LoadInput(input);
        auto values = RunCircuit(gates);
        return values["a"];
    }

    int Solve_2(const std::filesystem::path& input)
    {
        auto gates = LoadInput(input);

        auto gates_copy = gates;
        auto values = RunCircuit(gates_copy);

        // override 'b' to the signal from 'a'
        std::erase_if(gates, [](const Gate& g) { return g.op == Op::Const && g.out == "b"; });
        gates.emplace_back(Op::Const, Arg(values["a"]), Arg{}, "b");

        values = RunCircuit(gates);
        return values["a"];
    }

    REGISTER_SOLUTION(2015, 7, 1, Solve_1);
    REGISTER_SOLUTION(2015, 7, 2, Solve_2);
}

