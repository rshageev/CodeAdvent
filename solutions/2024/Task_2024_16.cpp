#include "Runner.h"

import std;
import utils;

namespace
{
    struct Node {
        Point pos;
        Direction dir;

        bool operator==(const Node&) const = default;
    };

    struct NodeHash {
        size_t operator() (Node n) const {
            int64 v = n.dir + 1000ll * n.pos.x + 1000'000ll * n.pos.y;
            return std::hash<int64>{}(v);
        }
    };

    struct NodeData {
        int score = 0;
        std::vector<Node> prev;
    };

    using Nodes = std::unordered_map<Node, NodeData, NodeHash>;

    bool AddNode(Nodes& nodes, Node node, int score, Node prev)
    {
        auto [itr, inserted] = nodes.try_emplace(node);
        if (inserted) {
            // not visited previously
            itr->second.score = score;
            itr->second.prev.push_back(prev);
            return true;
        }
        else if (score == itr->second.score) {
            // already visited but at the same distance, remember alternative route
            itr->second.prev.push_back(prev);
        }
        return false;
    }

    std::vector<Node> UpdateState(Node prev, const Array2D<char>& map, Nodes& nodes)
    {
        std::vector<Node> ns;

        int score = nodes.at(prev).score;

        Node next_fwd = { MovePoint(prev.pos, prev.dir), prev.dir };
        if (map.Contains(next_fwd.pos) && map[next_fwd.pos] != '#') {
            if (AddNode(nodes, next_fwd, score + 1, prev)) {
                ns.push_back(next_fwd);
            }
        }

        Node next_rl = { prev.pos, RotateLeft(prev.dir) };
        if (AddNode(nodes, next_rl, score + 1000, prev)) {
            ns.push_back(next_rl);
        }

        Node next_rr = { prev.pos, RotateRight(prev.dir) };
        if (AddNode(nodes, next_rr, score + 1000, prev)) {
            ns.push_back(next_rr);
        }

        return ns;
    }

    void ReconstructPath(Node end, Array2D<char>& map, const Nodes& nodes)
    {
        std::queue<Node> q;
        q.push(end);
        while (!q.empty()) {
            Node node = q.front();
            q.pop();

            map[node.pos] = 'O';
            q.push_range(nodes.at(node).prev);
        }
    }

    int CalcPath(Array2D<char>& map)
    {
        const Point start_pos = FindInArray2D(map, 'S');

        Nodes nodes = { {{start_pos, Dir::Right}, { 0 }} };

        auto is_better = [&nodes](Node n1, Node n2) {
            return nodes.at(n1).score > nodes.at(n2).score;
        };

        std::priority_queue<Node, std::vector<Node>, decltype(is_better)> q(is_better);
        q.push({ start_pos, Dir::Right });

        int shortest = std::numeric_limits<int>::max();

        while (!q.empty())
        {
            Node node = q.top();
            q.pop();

            auto& node_data = nodes.at(node);
            if (node_data.score > shortest) continue;

            if (map[node.pos] == 'E')
            {
                shortest = node_data.score;

                ReconstructPath(node, map, nodes);
            }
            else
            {
                q.push_range(UpdateState(node, map, nodes));
            }
        }

        return shortest;
    }

    int64 Solve_1(const std::filesystem::path& input)
    {
        auto map = ReadArray2D(input);
        return CalcPath(map);
    }

    int64 Solve_2(const std::filesystem::path& input)
    {
        auto map = ReadArray2D(input);
        CalcPath(map);
        return stdr::count(map, 'O');
    }

    REGISTER_SOLUTION(2024, 16, 1, Solve_1);
    REGISTER_SOLUTION(2024, 16, 2, Solve_2);
}

