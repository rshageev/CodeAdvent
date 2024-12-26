#include "Runner.h"

import std;
import utils;

namespace
{
    struct Data
    {
        using NodeMap = std::map<std::string, std::pair<std::string, std::string>>;
        using Node = NodeMap::const_iterator;

        std::string directions;
        NodeMap nodes;
    };

    Data LoadInputData(const std::filesystem::path& input)
    {
        Data data;

        const auto lines = ReadLines(input);

        data.directions = lines[0];

        for (const auto& line : lines | stdv::drop(2) | stdv::filter(not_empty))
        {
            auto from = line.substr(0, 3);
            auto left = line.substr(7, 3);
            auto right = line.substr(12, 3);
            data.nodes.emplace(from, std::make_pair(left, right));
        }

        return data;
    }

    template<class Pred>
    auto StepsFromTo(const Data& data, Data::Node from, Pred&& is_finished)
    {
        size_t steps = 0;
        auto dir = data.directions.begin();
        auto current = from;

        while (current != data.nodes.end() && !is_finished(current))
        {
            current = data.nodes.find((*dir == 'L') ? current->second.first : current->second.second);

            ++dir;
            if (dir == data.directions.end()) {
                dir = data.directions.begin();
            }
            ++steps;
        }

        return std::make_pair(current, steps);
    }

    size_t Solve_1(const std::filesystem::path& input)
    {
        const auto data = LoadInputData(input);

        auto start_node = data.nodes.find("AAA");
        auto is_finished = [](Data::Node node) { return node->first == "ZZZ"; };

        auto [node, steps] = StepsFromTo(data, start_node, is_finished);
        return steps;
    }

    size_t Solve_2(const std::filesystem::path& input)
    {
        /* Solution is not technically correct in general, but works for this input */

        const auto data = LoadInputData(input);

        size_t result = 1;

        auto is_finished = [](Data::Node n) { return n->first.ends_with('Z'); };

        for (auto itr = data.nodes.begin(); itr != data.nodes.end(); ++itr)
        {
            if (itr->first.ends_with('A'))
            {
                auto [node, steps] = StepsFromTo(data, itr, is_finished);
                result = std::lcm(steps, result);
            }
        }

        return result;
    }

    REGISTER_SOLUTION(2023, 8, 1, Solve_1);
    REGISTER_SOLUTION(2023, 8, 2, Solve_2);
}