#include "Runner.h"

import std;
import utils;

namespace
{
    struct PartNumber
    {
        int value;
        Rect rect;

        PartNumber(int v, int line, size_t pos, size_t w)
            : value(v)
        {
            rect.x = static_cast<int>(pos) - 1;
            rect.w = static_cast<int>(w) + 2;
            rect.y = line - 1;
            rect.h = 3;
        }
    };

    void ParseLine(std::string_view str, int line_n, std::vector<PartNumber>& out)
    {
        int value;
        size_t pos = 0;

        while (!str.empty())
        {
            auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
            if (ec == std::errc{}) {
                // got a number
                size_t w = ptr - str.data();
                if (value > 0)
                    out.emplace_back(value, line_n, pos, w);
                else
                    out.emplace_back(-value, line_n, pos + 1, w - 1);
                pos += w;
                str.remove_prefix(w);
            }
            else {
                // not a number
                ++pos;
                str.remove_prefix(1);
            }
        }
    }

    std::vector<PartNumber> ReadPartNumbers(const std::filesystem::path& input)
    {
        std::vector<PartNumber> part_numbers;
        int line_n = 0;
        for (const auto& line : ReadLines(input)) {
            ParseLine(line, line_n++, part_numbers);
        }
        return part_numbers;
    }

    template<class Pred>
    std::vector<Point> ReadSymbolPositions(const std::filesystem::path& input, Pred&& pred)
    {
        std::vector<Point> out;
        int y = 0;
        for (const auto& line : ReadLines(input))
        {
            int x = 0;
            for (char ch : line) {
                if (pred(ch)) out.emplace_back(x, y);
                ++x;
            }
            ++y;
        }
        return out;
    }

    bool IsPartValid(const Rect& part_rect, std::span<const Point> all_parts)
    {
        for (Point pt : all_parts) {
            if (Contains(part_rect, pt))
                return true;
        }
        return false;
    }

    int Solve_1(const std::filesystem::path& input)
    {
        auto part_numbers = ReadPartNumbers(input);

        auto is_part = [](char ch) { return ch != '.' && !is_digit(ch); };
        auto parts = ReadSymbolPositions(input, is_part);

        int sum = 0;
        for (const auto& pn : part_numbers) {
            if (IsPartValid(pn.rect, parts)) {
                sum += pn.value;
            }
        }

        return sum;
    }

    std::optional<int> FindGearRatio(Point pos, std::span<const PartNumber> part_numbers)
    {
        int ratio = 1;
        int count = 0;
        for (const auto& pn : part_numbers) {
            if (Contains(pn.rect, pos)) {
                ++count;
                ratio *= pn.value;
            }
        }
        if (count == 2)
            return { ratio };
        else
            return {};
    }

    int Solve_2(const std::filesystem::path& input)
    {
        auto part_numbers = ReadPartNumbers(input);

        auto is_gear = [](char ch) { return ch == '*'; };
        auto gears = ReadSymbolPositions(input, is_gear);

        int sum = 0;
        for (auto g : gears) {
            auto ratio = FindGearRatio(g, part_numbers);
            if (ratio) {
                sum += *ratio;
            }
        }

        return sum;
    }

    REGISTER_SOLUTION(2023, 3, 1, Solve_1);
    REGISTER_SOLUTION(2023, 3, 2, Solve_2);
}
