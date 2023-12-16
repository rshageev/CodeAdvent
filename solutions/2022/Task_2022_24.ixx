export module AoC_2022.Day24;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

constexpr Direction Wall = 0b1000'0000;

Direction CharToDirW(char ch)
{
    return (ch == '#') ? Wall : CharToDir(ch);
}

struct Field
{
    Point entrance;
    Point exit;
    Array2D<Direction> blizzards;
};

using PlayerPos = std::set<Point>;

Field LoadField(const std::filesystem::path& input)
{
    Field field;
    field.blizzards = ReadArray2D(input, CharToDirW);
    field.entrance = { 1, field.blizzards.Area().h - 1 };
    field.exit = { field.blizzards.Area().w - 2, 0 };
    return field;
}

Array2D<Direction> MoveBlizzards(const Array2D<Direction>& blizzards)
{
    auto next_blizzards = Array2D<Direction>(blizzards.Area(), Dir::None);
    const Rect blizzards_area = Inflated(blizzards.Area(), -1, -1);

    for (Point pos : to_cell_coords(blizzards))
    {
        auto cell = blizzards[pos];
        if (cell == Wall) {
            next_blizzards[pos] = Wall;
        } else {
            for (auto [dir, off] : Neighbours4) {
                if (cell & dir) {
                    const Point new_pos = WrapPoint(pos + off, blizzards_area);
                    next_blizzards[new_pos] |= dir;
                }
            }
            
        }
    }

    return next_blizzards;
}

PlayerPos MovePlayer(const PlayerPos& current, const Array2D<Direction>& blizzards)
{
    PlayerPos next;

    constexpr Point offsets[] = { {0, 0}, {-1, 0}, {1, 0}, {0, 1}, {0, -1}};
    for (Point pos : current)
    {
        for (Point off : offsets)
        {
            const Point new_pos = pos + off;
            if (Contains(blizzards.Area(), new_pos) && blizzards[new_pos] == Dir::None)
            {
                next.insert(new_pos);
            }
        }
    }

    return next;
}

void UpdateField(Field& field, PlayerPos& player_pos)
{
    auto next_blizzards = MoveBlizzards(field.blizzards);
    auto next = MovePlayer(player_pos, next_blizzards);

    field.blizzards = std::move(next_blizzards);
    player_pos = std::move(next);
}

int MoveFromTo(Field& field, Point from, Point to)
{
    PlayerPos pos = { from };
    int counter = 0;
    while (!pos.contains(to)) {
        UpdateField(field, pos);
        ++counter;
    }

    return counter;
}

int Solve_1(const std::filesystem::path& input)
{
    auto field = LoadField(input);

    return MoveFromTo(field, field.entrance, field.exit);
}

int Solve_2(const std::filesystem::path& input)
{
    auto field = LoadField(input);

    const auto m1 = MoveFromTo(field, field.entrance, field.exit);
    const auto m2 = MoveFromTo(field, field.exit, field.entrance);
    const auto m3 = MoveFromTo(field, field.entrance, field.exit);

    return m1 + m2 + m3;
}