#include <scn/scan.h>
#include "Runner.h"

import std;
import utils;

namespace
{
    auto LoadGraph(const std::filesystem::path& input)
    {
        std::unordered_set<std::string> nodes;
        std::unordered_set<std::pair<std::string_view, std::string_view>, PairHash> edges;

        for (const auto& line : ReadLines(input)) {
            std::string s1 = line.substr(0, 2);
            std::string s2 = line.substr(3, 2);
            std::string_view n1 = *nodes.insert(s1).first;
            std::string_view n2 = *nodes.insert(s2).first;
            edges.emplace(n1, n2);
            edges.emplace(n2, n1);
        }

        return std::pair(std::move(nodes), std::move(edges));
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        const auto [nodes, edges] = LoadGraph(input);

        std::set<std::array<std::string_view, 3>> cliques;

        for (const auto& [n1, n2] : edges) {
            for (const auto& n3 : nodes) {
                if (n1 != n3 && n2 != n3
                    && (n1[0] == 't' || n2[0] == 't' || n3[0] == 't')
                    && edges.contains(std::pair(n1, n3))
                    && edges.contains(std::pair(n2, n3)))
                {
                    std::array<std::string_view, 3> clique = { n1,n2,n3 };
                    stdr::sort(clique);
                    cliques.insert(clique);
                }
            }
        }

        return cliques.size();
    }

    std::string Solve_2(const std::filesystem::path& input)
    {
        const auto [nodes, edges] = LoadGraph(input);

        std::set<std::string_view> max_clique;

        for (const auto& clique_start : nodes)
        {
            std::set<std::string_view> clique = { clique_start };

            bool added = true;
            for (const auto& pn : nodes)
            {
                // can the node be added?
                bool can_be_added = true;
                for (const auto& cn : clique) {
                    if (!edges.contains(std::pair(pn, cn))) {
                        can_be_added = false;
                        break;
                    }
                }

                if (can_be_added) {
                    clique.insert(pn);
                }
            }

            if (clique.size() > max_clique.size()) {
                max_clique = std::move(clique);
            }
        }

        return max_clique | stdv::join_with(',') | stdr::to<std::string>();;
    }

    REGISTER_SOLUTION(2024, 23, 1, Solve_1);
    REGISTER_SOLUTION(2024, 23, 2, Solve_2);
}