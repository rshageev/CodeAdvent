export module AoC_2022.Day24;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

constexpr Direction Wall = 16;

Direction CharToDir(char ch)
{
    switch (ch) {
        case '.': return Dir::None;
        case '<': return Dir::Left;
        case '>': return Dir::Right;
        case '^': return Dir::Up;
        case 'v': return Dir::Down;
        default: return Wall;
    }
}

char DirToChar(Direction dir)
{
    switch (dir) {
        case Wall: return '#';
        case Dir::None: return '.';
        case Dir::Left: return '<';
        case Dir::Right: return '>';
        case Dir::Up: return '^';
        case Dir::Down: return 'v';
    }
    return '0' + std::popcount(dir);
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
    const auto file_data = ReadText(input);

    field.blizzards = Array2DFromString(file_data, CharToDir);

    field.entrance = { 1, field.blizzards.Area().h - 1 };
    field.exit = { field.blizzards.Area().w - 2, 0 };

    return field;
}

Array2D<Direction> MoveBlizzards(const Array2D<Direction>& blizzards)
{
    auto next_blizzards = Array2D<Direction>(blizzards.Area(), Dir::None);
    const Rect blizzards_area = Inflated(blizzards.Area(), -1, -1);

    static constexpr std::pair<Direction, Point> directions[] = {
        { Dir::Left, {-1, 0}},
        { Dir::Right, {1, 0}},
        { Dir::Up, {0, 1}},
        { Dir::Down, {0, -1}},
    };

    for (Point pos : to_cell_coords(blizzards))
    {
        auto cell = blizzards[pos];
        if (cell == Wall) {
            next_blizzards[pos] = Wall;
        } else {
            for (auto [dir, off] : directions) {
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