export module AoC_2023.Day2;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

constexpr int CubeIdxFromString(std::string_view str)
{
    if (str == "red") return 0;
    if (str == "green") return 1;
    if (str == "blue") return 2;
    return -1;
}

using CubeSet = std::array<int, 3>;

struct Game
{
    int index = 0;
    std::vector<CubeSet> sets;
};

Game ParseLine(std::string_view str)
{
    Game game;

    Skip(str, "Game ");
    game.index = Read<int>(str);
    Skip(str, ": ");

    game.sets.push_back({0, 0, 0});

    while (true)
    {
        const int count = Read<int>(str);
        Skip(str, " ");
        const auto color_str = ReadWord(str);
        const auto color_idx = CubeIdxFromString(color_str);
        game.sets.back()[color_idx] += count;

        if (Skip(str, ", ")) {
            // next color
        } else if (Skip(str, "; ")) {
            // next set
            game.sets.push_back({ 0, 0, 0 });
        } else {
            // end of line
            break;
        }
    }

    return game;
}

bool IsGamePossible(const Game& game, const CubeSet& bag)
{
    auto set_valid = [&bag](const CubeSet& set) {
        return stdr::all_of(stdv::zip(set, bag), apply_to_pair<std::less_equal<>>{});
    };
    return stdr::all_of(game.sets, set_valid);
}

int Solve_1(const std::filesystem::path& input)
{
    constexpr CubeSet bag{ 12, 13, 14 };

    int result = 0;
    for (const auto& line : ReadLines(input))
    {
        const auto game = ParseLine(line);
        const bool is_valid = IsGamePossible(game, bag);
        if (is_valid) {
            result += game.index;
        }
    }
    return result;
}


CubeSet MinimumSet(const Game& game)
{
    CubeSet min_set = { 0, 0, 0 };
    for (const auto& set : game.sets) {
        for (auto [c, mc] : stdv::zip(set, min_set)) {
            mc = std::max(c, mc);
        }
    }
    return min_set;
}

int SetPower(CubeSet set)
{
    return set[0] * set[1] * set[2];
}

int Solve_2(const std::filesystem::path& input)
{
    int result = 0;
    for (const auto& line : ReadLines(input))
    {
        const auto game = ParseLine(line);
        const auto min_set = MinimumSet(game);
        const auto set_power = SetPower(min_set);
        result += set_power;
    }
    return result;
}