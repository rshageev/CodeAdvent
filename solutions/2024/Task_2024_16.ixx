module;
#include <scn/scan.h>

export module AoC_2024.Day16;

import std;
import utils;

export int64 Solve_1(const std::filesystem::path& input);
export int64 Solve_2(const std::filesystem::path& input);

module : private;

namespace
{

    struct State
    {
        Point pos;
        Direction dir = Dir::Right;
        int score = 0;
        int index = 0;
        int prev = -1;

        bool operator<(const State& rhs) const noexcept { return score > rhs.score; }
    };

    struct VisitedMap
    {
        bool Check(Point pos, Direction dir, int score)
        {
            auto [itr, inserted] = visited.try_emplace(std::pair(pos, dir), score);
            if (inserted) {
                return true;
            } else if (score <= itr->second) {
                itr->second = score;
                return true;
            }
            return false;
        }

        std::map<std::pair<Point, Direction>, int> visited;
    };

    std::vector<State> UpdateState(State s, const Array2D<char>& map, VisitedMap& visited, std::vector<State>& all_states)
    {
        std::vector<State> ns;

        Point np = MovePoint(s.pos, s.dir);
        if (map.Contains(np) && map[np] != '#' && visited.Check(np, s.dir, s.score + 1)) {
            State next{ np, s.dir, s.score + 1, all_states.size(), s.index };
            all_states.push_back(next);
            ns.push_back(next);
        }

        if (visited.Check(s.pos, RotateLeft(s.dir), s.score + 1000)) {
            State r1{ s.pos, RotateLeft(s.dir), s.score + 1000, all_states.size(), s.index };
            all_states.push_back(r1);
            ns.push_back(r1);
        }

        if (visited.Check(s.pos, RotateRight(s.dir), s.score + 1000)) {
            State r2{ s.pos, RotateRight(s.dir), s.score + 1000, all_states.size(), s.index };
            all_states.push_back(r2);
            ns.push_back(r2);
        }
        return ns;
    }

    int64 CalcPath(Array2D<char>& map)
    {
        const Point start_pos = FindInArray2D(map, 'S');

        VisitedMap visited;
        std::priority_queue<State> q;
        State init{ start_pos, Dir::Right, 0, 0, -1 };
        q.push(init);

        std::vector<State> all_states = { init };
        int64 shortest = std::numeric_limits<int64>::max();

        while (!q.empty())
        {
            auto st = q.top();
            q.pop();

            if (st.score > shortest) continue;

            if (map[st.pos] == 'E')
            {
                shortest = st.score;

                // reconstruct path
                State* node = &st;
                while (node->prev >= 0) {
                    node = &all_states[node->prev];
                    map[node->pos] = 'O';
                }
            }
            else
            {
                q.push_range(UpdateState(st, map, visited, all_states));
            }
        }

        return shortest;
    }

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
    return stdr::count(map, 'O') + 1;
}