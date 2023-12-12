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

struct Ctx
{
    Ctx(const Data& data)
        : str(data.str)
        , lengths(data.lengths)
    {
        cache.resize(data.str.size());
        for (auto& cc : cache) {
            cc.resize(data.lengths.size());
        }
    }

    std::string_view str;
    std::span<const size_t> lengths;

    std::vector<std::vector<std::optional<size_t>>> cache;
};

bool SequenceCanStartFrom(std::string_view data, size_t seq_length)
{
    return (data.size() >= seq_length) // enough string to fit the sequence
        && (data.find_first_not_of("#?") >= seq_length) // no '.' inside sequence
        && (data.size() == seq_length || data[seq_length] != '#'); // sequence doesn't continue afterwards
}

// find all possible start positions for sequence of given length
std::vector<size_t> FindAllStartPositions(const Ctx& ctx, size_t start_pos, size_t seq_idx)
{
    std::vector<size_t> out;

    std::string_view data = ctx.str;
    const auto seq_length = ctx.lengths[seq_idx];

    if (data.size() < seq_length + start_pos)
        return out;

    data.remove_prefix(start_pos);

    for (size_t i = 0; i <= data.size() - seq_length; ++i)
    {
        if (SequenceCanStartFrom(data.substr(i), seq_length)) {
            out.push_back(i + start_pos);
        }
        
        if (data[i] == '#') {
            break;
        }
    }

    return out;
}

size_t CalcAllCombinations(Ctx& ctx, size_t start_pos = 0, size_t seq_idx = 0)
{
    if (start_pos >= ctx.str.size() || seq_idx >= ctx.lengths.size()) {
        return 0;
    }

    // check cache first
    if (auto res = ctx.cache[start_pos][seq_idx]) {
        return *res;
    }


    size_t count = 0;
    const auto length = ctx.lengths[seq_idx];
    auto positions = FindAllStartPositions(ctx, start_pos, seq_idx);

    if (seq_idx == (ctx.lengths.size() - 1))
    {
        for (auto pos : positions) {
            if (ctx.str.find('#', pos + length) == std::string_view::npos) {
                ++count;
            }
        }
    }
    else
    {
        for (auto pos : positions) {
            count += CalcAllCombinations(ctx, pos + length + 1, seq_idx + 1);
        }
    }

    // store result in cache
    ctx.cache[start_pos][seq_idx] = count;

    return count;
}

size_t Solve_1(const std::filesystem::path& input)
{
    size_t sum = 0;
    for (const auto& line : ReadLines(input) | stdv::filter(not_empty))
    {
        const auto data = ParseLine(line);
        Ctx ctx(data);
        const auto res = CalcAllCombinations(ctx);
        sum += res;
    }
    return sum;
}

size_t Solve_2(const std::filesystem::path& input)
{
    size_t sum = 0;
    for (const auto& line : ReadLines(input) | stdv::filter(not_empty))
    {
        const auto data = Unfold(ParseLine(line));
        Ctx ctx(data);
        const auto res = CalcAllCombinations(ctx);
        sum += res;
    }
    return sum;
}