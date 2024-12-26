#include "Runner.h"

import std;
import utils;

namespace
{
    struct Hand
    {
        std::string cards;
        std::map<char, int> card_counts;

        explicit Hand(std::string_view str)
            : cards(str)
        {
            for (char ch : str) {
                auto [itr, inserted] = card_counts.emplace(ch, 0);
                itr->second++;
            }
        }
    };

    std::pair<Hand, int> ParseLine(std::string_view str)
    {
        const auto hand_str = ReadWord(str);
        Skip(str, " ");
        const auto bid = Read<int>(str);

        return { Hand(hand_str), bid };
    }

    int HandType(const Hand& hand)
    {
        int max_count = 0;
        for (auto [card, count] : hand.card_counts) {
            max_count = std::max(max_count, count);
        }

        if (hand.card_counts.size() == 1)
        {
            return 6; // five of a kind
        }
        else if (hand.card_counts.size() == 2)
        {
            if (max_count == 4) return 5; // four of a kind
            if (max_count == 3) return 4; // full house
        }
        else if (hand.card_counts.size() == 3)
        {
            if (max_count == 3) return 3; // three of a kind
            if (max_count == 2) return 2; // two pair
        }
        else if (hand.card_counts.size() == 4)
        {
            return 1; // one pair
        }
        else if (hand.card_counts.size() == 5)
        {
            return 0; // high card
        }
        return 0;
    }

    bool CompareHand(std::string_view card_table, const Hand& hand1, const Hand& hand2)
    {
        const auto type1 = HandType(hand1);
        const auto type2 = HandType(hand2);
        if (type1 != type2)
            return type1 < type2;

        auto card_weaker = [=](char c1, char c2) {
            return card_table.find(c1) > card_table.find(c2);
            };
        return std::lexicographical_compare(
            hand1.cards.begin(), hand1.cards.end(),
            hand2.cards.begin(), hand2.cards.end(),
            card_weaker);
    }

    int CalcWinnings(auto&& hands)
    {
        int winnings = 0;
        int rank = 1;
        for (const auto& [h, b] : hands) {
            winnings += (b * rank++);
        }
        return winnings;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        constexpr std::string_view card_table = "AKQJT98765432";

        std::vector<std::pair<Hand, int>> hands;

        for (const auto& line : ReadLines(input)) {
            hands.push_back(ParseLine(line));
        }

        stdr::sort(hands, std::bind_front(CompareHand, card_table), tuple_get<0>);

        return CalcWinnings(hands);
    }

    void ApplyJokers(std::map<char, int>& cards)
    {
        auto jitr = cards.find('J');
        if (jitr == cards.end()) {
            return; // no jokers
        }

        int jokers = jitr->second;
        if (jokers >= 5) {
            return; // all cards are jokers
        }

        cards.erase(jitr);
        while (jokers > 0)
        {
            int* max_count = nullptr;
            for (auto& [card, count] : cards) {
                if (!max_count || count > *max_count) {
                    max_count = &count;
                }
            }

            int dc = std::min(jokers, 5 - *max_count);
            *max_count += dc;
            jokers -= dc;
        }
    }

    int Solve_2(const std::filesystem::path& input)
    {
        constexpr std::string_view card_table = "AKQT98765432J";

        std::vector<std::pair<Hand, int>> hands;

        for (const auto& line : ReadLines(input))
        {
            hands.push_back(ParseLine(line));
            ApplyJokers(hands.back().first.card_counts);
        }

        stdr::sort(hands, std::bind_front(CompareHand, card_table), tuple_get<0>);

        return CalcWinnings(hands);
    }

    REGISTER_SOLUTION(2023, 7, 1, Solve_1);
    REGISTER_SOLUTION(2023, 7, 2, Solve_2);
}