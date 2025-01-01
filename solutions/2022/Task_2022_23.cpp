#include "Runner.h"

import std;
import utils;

namespace
{
    using Elves = std::unordered_map<Point, std::optional<Point>>; // (current position, where we want to move)
    using Proposals = std::unordered_map<Point, int>; // how many elves want to move into specific position

    constexpr Direction MoveDirs[] = { Dir::N, Dir::S, Dir::W, Dir::E };

    auto LoadInput(const std::filesystem::path& input)
    {
        Elves elves;
        auto map = ReadArray2D(input);
        for (Point pt : to_cell_coords(map)) {
            if (map[pt] == '#') {
                elves.try_emplace(pt);
            }
        }
        return elves;
    }

    bool HasNeighbours(const Elves& elves, Point pos)
    {
        for (auto [_, off] : Neighbours8) {
            if (elves.contains(pos + off)) {
                return true;
            }
        }
        return false;
    }

    Proposals ProposeMove(Elves& elves, int start_dir)
    {
        Proposals prop;

        for (auto& [elf_pos, elf_next] : elves)
        {
            // check if we want to move at all
            if (!HasNeighbours(elves, elf_pos)) {
                continue;
            }

            // select one of the four directions to move
            for (int i = 0; i < 4; ++i)
            {
                Direction move_dir = MoveDirs[(start_dir + i) % 4];

                // check 3 neighbours in specific direction
                bool can_move = true;
                for (int i : { -1, 0, 1 }) {
                    Point np = MovePoint(elf_pos, std::rotr(move_dir, i));
                    if (elves.contains(np)) {
                        can_move = false;
                        break;
                    }
                }

                if (can_move) {
                    // found a free direction, make a proposal
                    Point np = MovePoint(elf_pos, move_dir);
                    prop[np]++;
                    elf_next = np;
                    break;
                }
            }
        }

        return prop;
    }

    bool MoveElves(Elves& elves, const Proposals& prop)
    {
        Elves new_elves;
        bool moved = false;
        for (auto [current_pos, desired_pos] : elves) {
            if (desired_pos) { // did we want to move?
                if (prop.at(*desired_pos) == 1) { // can we move?
                    new_elves.try_emplace(*desired_pos);
                    moved = true;
                }
                else {
                    new_elves.try_emplace(current_pos);
                }
            }
            else {
                new_elves.try_emplace(current_pos);
            }
        }
        elves = std::move(new_elves);
        return moved;
    }

    Rect BoundingRect(const Elves& elves)
    {
        auto [min_x, max_x] = stdr::minmax(elves | stdv::keys, std::less{}, &Point::x);
        auto [min_y, max_y] = stdr::minmax(elves | stdv::keys, std::less{}, &Point::y);

        return Rect{ min_x.x, min_y.y, max_x.x - min_x.x + 1, max_y.y - min_y.y + 1 };
    }

    int Solve_1(const std::filesystem::path& input)
    {
        auto elves = LoadInput(input);

        int dir = 0;
        for (int i = 0; i < 10; ++i) {
            auto prop = ProposeMove(elves, dir);
            MoveElves(elves, prop);
            dir = (dir + 1) % 4;
        }

        Rect rec = BoundingRect(elves);
        return rec.w * rec.h - (int)elves.size();
    }

    int Solve_2(const std::filesystem::path& input)
    {
        auto elves = LoadInput(input);

        int dir = 0;
        int step = 0;
        bool moving = true;
        while (moving) {
            auto prop = ProposeMove(elves, dir);
            moving = MoveElves(elves, prop);
            dir = (dir + 1) % 4;
            ++step;
        }
        return step;
    }

    REGISTER_SOLUTION(2022, 23, 1, Solve_1);
    REGISTER_SOLUTION(2022, 23, 2, Solve_2);
}