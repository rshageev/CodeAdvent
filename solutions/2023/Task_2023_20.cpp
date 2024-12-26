#include "Runner.h"

import std;
import utils;

namespace
{
    using Pulse = std::uint8_t;
    constexpr Pulse None = 0;
    constexpr Pulse Low = 1;
    constexpr Pulse High = 2;

    using Outputs = std::vector<std::string>;

    struct Broadcaster
    {
    };
    struct Output
    {
    };
    struct FlipFlop
    {
        bool state = false;
    };
    struct Conjunction
    {
        std::vector<std::pair<std::string, Pulse>> inputs;
    };
    using Module = std::variant<Output, Broadcaster, FlipFlop, Conjunction>;

    using ModulesMap = std::unordered_map<std::string, std::pair<Module, Outputs>>;


    std::pair<std::string, std::pair<Module, Outputs>> ParseLine(std::string_view str)
    {
        auto name = ReadWord(str, " ");
        Skip(str, " -> ");

        std::vector<std::string> out;
        while (!str.empty()) {
            out.emplace_back(ReadWord(str));
            Skip(str, ", ");
        }

        Module module;
        if (name.starts_with('%'))
        {
            module = FlipFlop{};
            name = name.substr(1);
        }
        else if (name.starts_with('&'))
        {
            module = Conjunction{};
            name = name.substr(1);
        }
        else if (name == "broadcaster")
        {
            module = Broadcaster{};
        }

        return std::pair<std::string, std::pair<Module, Outputs>>{ name, { module, out } };
    }

    auto LoadModules(const std::filesystem::path& input)
    {
        ModulesMap modules;

        for (const auto& line : ReadLines(input)) {
            modules.insert(ParseLine(line));
        }

        for (auto [name, mo] : modules)
        {
            for (const auto& out : mo.second)
            {
                // for every non existent output name, add special output module
                auto [itr, inserted] = modules.emplace(out, std::pair<Module, Outputs>{ Output{}, {} });;

                // set conjunction modules inputs
                auto& m = itr->second;
                if (auto* conj = std::get_if<Conjunction>(&m.first)) {
                    conj->inputs.emplace_back(name, Low);
                }
            }
        }

        return modules;
    }

    struct ReceivePulse
    {
        std::string_view from;
        Pulse in_pulse;

        Pulse operator() (Broadcaster& module) const
        {
            return in_pulse;
        }

        Pulse operator() (Output& module) const
        {
            return None;
        }

        Pulse operator() (FlipFlop& module) const
        {
            if (in_pulse == Low) {
                module.state = !module.state;
                return module.state ? High : Low;
            }
            return None;
        }

        Pulse operator() (Conjunction& module) const
        {
            bool all_high = true;
            for (auto& inputs : module.inputs) {
                if (inputs.first == from) {
                    inputs.second = in_pulse;
                }
                if (inputs.second != High) {
                    all_high = false;
                }
            }
            return all_high ? Low : High;
        }
    };

    void SendPulse(ModulesMap& modules, auto&& on_pulse)
    {
        std::queue<std::tuple<std::string, std::string, Pulse>> q;
        q.emplace("button", "broadcaster", Low);

        while (!q.empty())
        {
            auto [from, to, pulse] = q.front();

            on_pulse(from, to, pulse);

            auto& [recv_module, outputs] = modules.at(to);
            ReceivePulse vis{ from, pulse };
            const Pulse out_pulse = std::visit(vis, recv_module);

            if (out_pulse != None) {
                for (const auto& out_module : outputs) {
                    q.emplace(to, out_module, out_pulse);
                }
            }

            q.pop();
        }
    }

    size_t Solve_1(const std::filesystem::path& input)
    {
        auto modules = LoadModules(input);

        size_t low_sent = 0;
        size_t high_sent = 0;

        auto count_lh = [&](std::string_view from, std::string_view to, Pulse pulse) {
            if (pulse == Low) ++low_sent;
            if (pulse == High) ++high_sent;
            };

        for (int i = 0; i < 1000; ++i) {
            SendPulse(modules, count_lh);
        }

        return low_sent * high_sent;
    }

    size_t Solve_2(const std::filesystem::path& input)
    {
        auto modules = LoadModules(input);

        size_t counter = 0;

        std::map<std::string, size_t> bb_inputs;
        auto& [bb, out] = modules.at("bb");
        if (auto* conj = std::get_if<Conjunction>(&bb)) {
            for (auto [in, st] : conj->inputs) {
                bb_inputs.emplace(in, 0);
            }
        }

        auto track_bb = [&](const std::string& from, std::string_view to, Pulse pulse)
            {
                if (to == "bb" && pulse == High) {
                    auto& in = bb_inputs[from];
                    if (in == 0) in = counter;
                }
            };

        while (!stdr::all_of(bb_inputs, [](const auto& p) { return p.second > 0; }))
        {
            ++counter;
            SendPulse(modules, track_bb);
        }

        size_t result = 1;
        for (auto [name, cycle] : bb_inputs) {
            result = std::lcm(result, cycle);
        }

        return result;
    }

    REGISTER_SOLUTION(2023, 20, 1, Solve_1);
    REGISTER_SOLUTION(2023, 20, 2, Solve_2);
}