#include "Runner.h"

import std;
import utils;

namespace
{
    struct Ingredient {
        int capacity;
        int durability;
        int flavour;
        int texture;
        int calories;
    };

    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<Ingredient> ingredients;
        const std::regex regex(R"(\w+: capacity (-?\d+), durability (-?\d+), flavor (-?\d+), texture (-?\d+), calories (-?\d+))");
        for (const auto& line : ReadLines(input)) {
            std::smatch match;
            if (std::regex_match(line, match, regex)) {
                ingredients.push_back({
                    .capacity = to_int(match[1]),
                    .durability = to_int(match[2]),
                    .flavour = to_int(match[3]),
                    .texture = to_int(match[4]),
                    .calories = to_int(match[5]),
                });
            }
        }
        return ingredients;
    }

    int CookieScore(std::span<const Ingredient> ingredients, std::span<const int> weights)
    {
        int capacity = 0;
        int durability = 0;
        int flavour = 0;
        int texture = 0;
        for (const auto& [ingr, w] : stdv::zip(ingredients, weights)) {
            capacity += ingr.capacity * w;
            durability += ingr.durability * w;
            flavour += ingr.flavour * w;
            texture += ingr.texture * w;
        }
        return std::max(capacity, 0) * std::max(durability, 0) * std::max(flavour, 0) * std::max(texture, 0);
    }

    int CookieCal(std::span<const Ingredient> ingredients, std::span<const int> weights)
    {
        int cal = 0;
        for (const auto& [ingr, w] : stdv::zip(ingredients, weights)) {
            cal += ingr.calories * w;
        }
        return cal;
    }

    std::generator<std::span<const int>> GeneratePermutations(int total, std::span<int> values)
    {
        if (values.size() == 1) {
            values[0] = total;
            co_yield values;
        }
        else
        {
            for (int i = 0; i <= total; ++i) {
                values[0] = i;
                for (const auto rem : GeneratePermutations(total - i, values.subspan<1>())) {
                    co_yield values;
                }
            }
        }
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        const auto data = LoadData(input);

        int max_score = 0;
        std::vector<int> weights(data.size(), 0);
        for (auto v : GeneratePermutations(100, weights)) {
            max_score = std::max(CookieScore(data, v), max_score);
        }

        return max_score;
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        const auto data = LoadData(input);

        int max_score = 0;
        std::vector<int> weights(data.size(), 0);
        for (auto v : GeneratePermutations(100, weights)) {
            if (CookieCal(data, weights) == 500) {
                max_score = std::max(CookieScore(data, v), max_score);
            }
        }

        return max_score;
    }

    REGISTER_SOLUTION(2015, 15, 1, Solve_1);
    REGISTER_SOLUTION(2015, 15, 2, Solve_2);
}
