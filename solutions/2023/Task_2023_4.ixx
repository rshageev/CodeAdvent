export module AoC_2023.Day4;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export size_t Solve_2(const std::filesystem::path& input);

module : private;

struct Card
{
    int number;
    std::vector<int> my_numbers;
    std::vector<int> win_numbers;
};

Card ParseLine(std::string_view str)
{
    Card card;

    Skip(str, "Card ");
    while (Skip(str, " ")) {};

    card.number = ReadInt(str);
    Skip(str, ": ");

    while (true) {
        while (Skip(str, " ")) {};
        if (Skip(str, "|")) break;
        card.my_numbers.push_back(ReadInt(str));
    }

    while (!str.empty()) {
        while (Skip(str, " ")) {};
        card.win_numbers.push_back(ReadInt(str));
    }

    return card;
}

int CountWinningNumbers(const Card& card)
{
    auto is_winning = [&card](int num) { return contains(card.win_numbers, num); };
    return static_cast<int>(stdr::count_if(card.my_numbers, is_winning));
}

int CardPoints(const Card& card)
{
    const int win_numbers = CountWinningNumbers(card);
    return (win_numbers <= 0) ? 0 : (1 << (win_numbers - 1));
}

int Solve_1(const std::filesystem::path& input)
{
    int sum = 0;
    for (const auto& line : ReadLines(input) | stdv::filter(not_empty))
    {
        const auto card = ParseLine(line);
        const auto points = CardPoints(card);
        sum += points;
    }
    return sum;
}

size_t Solve_2(const std::filesystem::path& input)
{
    std::vector<int> win_cards;
    for (const auto& line : ReadLines(input) | stdv::filter(not_empty)) {
        const auto card = ParseLine(line);
        const auto win_numbers = CountWinningNumbers(card);
        win_cards.push_back(win_numbers);
    }


    size_t owned = 0;

    std::vector<int> current;
    std::vector<int> next;

    for (int i = 0; i < std::ssize(win_cards); ++i) {
        next.push_back(i);
    }
    owned += next.size();

    while (!next.empty())
    {
        current = std::move(next);
        next.clear();
        
        for (int card_idx : current) {
            int win_numbers = win_cards[card_idx];
            for (int i = 0; i < win_numbers; ++i) {
                next.push_back(card_idx + i + 1);
            }
        }
        owned += next.size();
    }

    return owned;
}