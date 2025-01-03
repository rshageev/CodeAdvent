#include "Runner.h"

import std;
import utils;

namespace
{
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

        card.number = Read<int>(str);
        Skip(str, ": ");

        while (true) {
            while (Skip(str, " ")) {};
            if (Skip(str, "|")) break;
            card.my_numbers.push_back(Read<int>(str));
        }

        while (!str.empty()) {
            while (Skip(str, " ")) {};
            card.win_numbers.push_back(Read<int>(str));
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
        for (const auto& line : ReadLines(input))
        {
            const auto card = ParseLine(line);
            const auto points = CardPoints(card);
            sum += points;
        }
        return sum;
    }

    int Solve_2(const std::filesystem::path& input)
    {
        std::vector<int> win_cards;
        for (const auto& line : ReadLines(input)) {
            const auto card = ParseLine(line);
            const auto win_numbers = CountWinningNumbers(card);
            win_cards.push_back(win_numbers);
        }

        std::deque<int> queue;
        int owned = 0;

        auto queue_numbers = [&](int start, int count) {
            queue.append_range(stdv::iota(start, start + count));
            owned += count;
            };

        queue_numbers(0, static_cast<int>(win_cards.size()));

        while (!queue.empty())
        {
            const int card_idx = queue.front();
            const int win_numbers = win_cards[card_idx];
            queue_numbers(card_idx + 1, win_numbers);
            queue.pop_front();
        }

        return owned;
    }

    REGISTER_SOLUTION(2023, 4, 1, Solve_1);
    REGISTER_SOLUTION(2023, 4, 2, Solve_2);
}