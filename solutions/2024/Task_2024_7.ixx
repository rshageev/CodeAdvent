export module AoC_2024.Day7;

import std;
import utils;

export uint64 Solve_1(const std::filesystem::path& input);
export uint64 Solve_2(const std::filesystem::path& input);

module : private;

auto LoadData(const std::filesystem::path& input)
{
    std::vector<std::pair<uint64, std::vector<uint64>>> data;
    for (std::string_view line : ReadLines(input)) {
        auto& entry = data.emplace_back();
        entry.first = Read<uint64>(line);
        Skip(line, ": ");
        do {
            entry.second.push_back(Read<uint64>(line));
        } while (Skip(line, " "));
    }
    return data;
}

bool Check(uint64 expected_res, std::span<const uint64> values, std::string_view ops)
{
    uint64 result = values[0];
    for (auto [op, val] : stdv::zip(ops, values.subspan(1))) {
        switch (op) {
        case '*':
            result *= val; break;
        case '|':
            result = Read<uint64>(std::to_string(result) + std::to_string(val)); break;
        case '+':
            result += val; break;
        }
    }
    return expected_res == result;
}

bool Check(uint64 expected_res, std::span<const uint64> values, size_t muls, size_t concats = 0)
{
    std::string ops;
    ops.append(muls, '*');
    ops.append(concats, '|');
    ops.resize(values.size() - 1, '+');
    stdr::sort(ops);

    do {
        if (Check(expected_res, values, ops)) {
            return true;
        }
    } while (stdr::next_permutation(ops).found);

    return false;
}

bool CanBeSolved1(uint64 result, std::span<const uint64> values)
{
    for (size_t mn = 0; mn < values.size(); ++mn) {
        if (Check(result, values, mn)) {
            return true;
        }
    }
    return false;
}

bool CanBeSolved2(uint64 result, std::span<const uint64> values)
{
    for (size_t mn = 0; mn < values.size(); ++mn) {
        for (size_t cn = 0; cn < values.size() - mn; ++cn) {
            if (Check(result, values, mn, cn)) {
                return true;
            }
        }
    }
    return false;
}

uint64 Solve_1(const std::filesystem::path& input)
{
    uint64 result = 0;
    for (const auto& [res, vs] : LoadData(input)) {
        if (CanBeSolved1(res, vs)) {
            result += res;
        }
    }

    return result;
}

uint64 Solve_2(const std::filesystem::path& input)
{
    uint64 result = 0;
    for (const auto& [res, vs] : LoadData(input)) {
        if (CanBeSolved2(res, vs)) {
            result += res;
        }
    }
    return result;
}