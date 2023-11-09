export module AoC_2022.Day16;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

struct Valve
{
    int flow = 0;
    std::vector<std::string> tunnels;
};

using Valves = std::unordered_map<std::string, Valve>;

std::pair<std::string, Valve> ParseLine(std::string_view str)
{
    // Valve WL has flow rate=7; tunnels lead to valves OQ, VN, PU, VF, UA

    Valve v;
    Skip(str, "Valve ");
    auto name = ReadWord(str, " ");
    Skip(str, " has flow rate=");
    v.flow = ReadInt(str);
    Skip(str, "; tunnels lead to valves ");
    Skip(str, "; tunnel leads to valve ");

    v.tunnels.emplace_back(ReadWord(str, ","));
    while (Skip(str, ", ")) {
        v.tunnels.emplace_back(ReadWord(str, ","));
    }

    return { std::string(name), v };
}

Valves ReadValves(const std::filesystem::path& input)
{
    Valves valves;
    for (const auto& line : ReadLines(input) | stdv::filter(not_empty))
    {
        valves.insert(ParseLine(line));
    }
    return valves;
}

void Traverse(const Valves& valves, const std::string& current, int remaining_time, int pressure_released, std::vector<std::string_view> opened, int& max_pressure_released)
{
    if (remaining_time > 0)
    {
        const auto& valve = valves.at(current);

        // maybe open the valve
        if (valve.flow > 0 && (stdr::find(opened, std::string_view(current)) == stdr::end(opened))) {         
            --remaining_time;
            pressure_released += remaining_time * valve.flow;
            opened.emplace_back(current);
        }

        // traverse tunnels
        if (remaining_time > 1)
        {
            for (const auto& next_valve : valve.tunnels)
            {
                Traverse(valves, next_valve, remaining_time - 1, pressure_released, opened, max_pressure_released);
            }
        }
        else
        {
            if (pressure_released > max_pressure_released) {
                max_pressure_released = pressure_released;
            }
        }
    }
}

int Solve_1(const std::filesystem::path& input)
{
    const auto valves = ReadValves(input);
    int max_pressure = 0;
    Traverse(valves, "AA", 30, 0, {}, max_pressure);
    return max_pressure;
}

int Solve_2(const std::filesystem::path& input)
{
    return 0;
}