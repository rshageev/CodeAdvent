export module AoC_2023.Day17;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

using Steps = std::int16_t;
using Loss = std::int16_t;
using LossMap = Array2D<Loss>;

template<class S, class InitState, class P1, class P2, class Comp>
S Dijkstra(InitState&& initial_states,
    P1&& move_state,
    P2&& is_finished,
    Comp&& is_better)
{
    std::priority_queue<S, std::vector<S>, Comp> states(is_better);
    states.push_range(initial_states);

    while (!states.empty())
    {
        auto st = states.top();     
        if (is_finished(st)) {
            return st;
        } else {
            states.pop();
            states.push_range(move_state(st));
        }
    }
    return {};
}

struct State
{
    Point pos;
    Direction dir;
    Steps steps = 0;
    Loss loss = 0;
};

struct VisitedMap
{
    bool Check(const State& st)
    {
        return visited.emplace(st.pos, st.dir, st.steps).second;
    }

    std::set<std::tuple<Point, Direction, Steps>> visited;
};


State MoveState(const State& prev, Direction dir, Steps steps, const LossMap& loss_map)
{
    State st{ prev.pos, dir, steps, prev.loss };

    if (dir == prev.dir) st.steps += prev.steps;

    for (int i = 0; i < steps; ++i) {
        st.pos = MovePoint(st.pos, st.dir);
        st.loss += loss_map[st.pos];
    }
    return st;
}

std::vector<State> UpdateState(const State& st, const LossMap& loss_map, VisitedMap& visited, Steps min_straight, Steps max_straight)
{
    std::vector<State> out;

    if (st.steps < max_straight)
    {
        const Steps steps = std::max(1, min_straight - st.steps);
        auto nst = MoveState(st, st.dir, steps, loss_map);
        if (loss_map.Contains(nst.pos) && visited.Check(nst)) {
            out.push_back(nst);
        }
    }

    if (st.steps >= min_straight)
    {
        for (Direction dir : { RotateLeft(st.dir), RotateRight(st.dir) })
        {
            auto nst = MoveState(st, dir, min_straight, loss_map);
            if (loss_map.Contains(nst.pos) && visited.Check(nst)) {
                out.push_back(nst);
            }
        }
    }

    return out;
}

int Solve(const std::filesystem::path& input, int min_straight, int max_straight)
{
    const auto loss_map = ReadArray2D(input, [](char ch) -> Loss { return ch - '0'; });
    const auto start = LeftTop(loss_map.Area());
    const auto finish = RightBottom(loss_map.Area());

    VisitedMap visited;

    auto is_better = [](const State& st1, const State& st2) { return st1.loss > st2.loss; };

    auto is_finished = [finish](const State& st) { return st.pos == finish; };

    auto move_state = [&](const State& st) { return UpdateState(st, loss_map, visited, min_straight, max_straight); };

    State init[] = { { start, Dir::Right }, { start, Dir::Down } };

    const auto final_state = Dijkstra<State>(init, move_state, is_finished, is_better);

    return final_state.loss;
}


int Solve_1(const std::filesystem::path& input)
{
    return Solve(input, 1, 3);
}

int Solve_2(const std::filesystem::path& input)
{
    return Solve(input, 4, 10);
}