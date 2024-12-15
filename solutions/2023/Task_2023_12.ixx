export module AoC_2023.Day12;

import std;
import utils;

export size_t Solve_1(const std::filesystem::path& input);
export size_t Solve_2(const std::filesystem::path& input);

module : private;

struct Data
{
    std::string str;
    std::vector<size_t> lengths;

    mutable std::vector<std::vector<std::optional<size_t>>> cache;
};

Data ParseLine(std::string_view str)
{
    Data out;
    out.str = ReadWord(str, " ");
    Skip(str, " ");
    while (!str.empty()) {
        out.lengths.push_back(Read<size_t>(str));
        Skip(str, ",");
    }
    return out;
}

Data Unfold(const Data& data)
{
    Data out;
    out.str = stdv::repeat(data.str, 5) | stdv::join_with('?') | stdr::to<std::string>();
    out.lengths = stdv::repeat(data.lengths, 5) | stdv::join | stdr::to<std::vector>();
    return out;
}

bool SequenceCanStartFrom(std::string_view data, size_t start_pos, size_t seq_length)
{
    const auto end_pos = start_pos + seq_length;
    return (data.size() >= end_pos) // enough string to fit the sequence
        && (data.find_first_not_of("#?", start_pos) >= end_pos) // no '.' inside sequence
        && (data.size() == end_pos || data[end_pos] != '#'); // sequence doesn't continue afterwards
}

// find all possible start positions for sequence of given length
std::vector<size_t> FindAllStartPositions(const Data& data, size_t start_pos, size_t seq_idx)
{
    std::vector<size_t> out;

    const auto seq_length = data.lengths[seq_idx];

    if (data.str.size() < seq_length + start_pos)
        return out;

    for (size_t i = start_pos; i <= data.str.size() - seq_length; ++i)
    {
        if (SequenceCanStartFrom(data.str, i, seq_length)) {
            out.push_back(i);
        }
        
        if (data.str[i] == '#') {
            break;
        }
    }

    return out;
}

size_t CalcAllCombinations(const Data& data, size_t start_pos = 0, size_t seq_idx = 0)
{
    if (start_pos >= data.str.size() || seq_idx >= data.lengths.size()) {
        return 0;
    }

    // init cache
    if (data.cache.empty()) {
        data.cache.resize(data.str.size());
        for (auto& cc : data.cache) {
            cc.resize(data.lengths.size());
        }
    }

    // check cache first
    if (auto res = data.cache[start_pos][seq_idx]) {
        return *res;
    }


    size_t count = 0;
    const auto length = data.lengths[seq_idx];
    auto positions = FindAllStartPositions(data, start_pos, seq_idx);

    if (seq_idx == (data.lengths.size() - 1))
    {
        for (auto pos : positions) {
            if (data.str.find('#', pos + length) == std::string::npos) {
                ++count;
            }
        }
    }
    else
    {
        for (auto pos : positions) {
            count += CalcAllCombinations(data, pos + length + 1, seq_idx + 1);
        }
    }

    // store result in cache
    data.cache[start_pos][seq_idx] = count;

    return count;
}

size_t Solve_1(const std::filesystem::path& input)
{
    size_t sum = 0;
    for (const auto& line : ReadLines(input))
    {
        const auto data = ParseLine(line);
        const auto res = CalcAllCombinations(data);
        sum += res;
    }
    return sum;
}

size_t Solve_2(const std::filesystem::path& input)
{
    size_t sum = 0;
    for (const auto& line : ReadLines(input))
    {
        const auto data = Unfold(ParseLine(line));
        const auto res = CalcAllCombinations(data);
        sum += res;
    }
    return sum;
}