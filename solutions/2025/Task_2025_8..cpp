#include "Runner.h"

import std;
import utils;

namespace
{
    using Pos = TVec3<int64>;

    auto ReadData(const std::filesystem::path& input)
    {
        std::vector<Pos> nodes;
        for (std::string_view line : ReadLines(input)) {
            Pos pos;
            Parse(line).Read(pos.x).Skip(",").Read(pos.y).Skip(",").Read(pos.z);
            nodes.push_back(pos);
        }

        std::vector<std::tuple<int64, int, int>> distances;
        for (int i = 0; i < nodes.size() - 1; ++i) {
            for (int j = i + 1; j < nodes.size(); ++j) {
                int64 dist = SqDist(nodes[i], nodes[j]);
                distances.emplace_back(dist, i, j);
            }
        }
        stdr::sort(distances);

        return std::pair(std::move(nodes), std::move(distances));
    }

    int CircuitSize(const std::multimap<int, int>& connections, int start_node_idx, std::set<int>& visited)
    {
        int size = 1;

        std::queue<int> q;
        q.push(start_node_idx);
        visited.insert(start_node_idx);

        while (!q.empty()) {
            int node_idx = q.front();
            q.pop();

            auto [first, last] = connections.equal_range(node_idx);
            for (auto itr = first; itr != last; ++itr) {
                if (!visited.contains(itr->second)) {
                    q.push(itr->second);
                    visited.insert(itr->second);
                    ++size;
                }
            }
        }
        
        return size;
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        auto [nodes, distances] = ReadData(input);

        const int N = (input.filename() == "input.txt") ? 1000 : 10;

        std::multimap<int, int> connections;
        for (auto [dist, i, j] : distances | stdv::take(N)) {
            connections.emplace(i, j);
            connections.emplace(j, i);
        }

        std::set<int> visited;
        std::vector<int> sizes;
        for (auto [node_idx, _] : connections) {
            if (!visited.contains(node_idx)) {
                sizes.push_back(CircuitSize(connections, node_idx, visited));
            }
        }

        stdr::sort(sizes, std::greater{});
        int64 result = 1;
        for (auto sz : sizes | stdv::take(3)) {
            result *= sz;
        }

        return result;
    }

    int64 Solve_2(const std::filesystem::path& input)
    {
        auto [nodes, distances] = ReadData(input);

        std::multimap<int, int> connections;

        for (auto [dist, i, j] : distances) {
            connections.emplace(i, j);
            connections.emplace(j, i);
            std::set<int> visited;
            if (CircuitSize(connections, 0, visited) >= nodes.size()) {
                return nodes[i].x * nodes[j].x;
            }
        }

        return 0;
    }

    REGISTER_SOLUTION(2025, 8, 1, Solve_1);
    REGISTER_SOLUTION(2025, 8, 2, Solve_2);
}
