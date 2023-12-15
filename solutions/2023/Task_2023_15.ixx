export module AoC_2023.Day15;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export size_t Solve_2(const std::filesystem::path& input);

module : private;

constexpr int Hash(std::string_view str)
{
    int hash = 0;
    for (char ch : str) {
        hash += ch;
        hash *= 17;
        hash = hash % 256;
    }
    return hash;
}

struct Lens {
    std::string_view label;
    int fl;
};

std::pair<Lens, char> ParseEntry(std::string_view str)
{
    const auto label = ReadWord(str, "=-");
    char op = str[0];
    int length = 0;
    if (op == '=') {
        str.remove_prefix(1);
        length = Read<int>(str);
    }
    return { { label, length}, op };
}

int Solve_1(const std::filesystem::path& input)
{
    const auto text = ReadText(input);

    int sum = 0;
    for (auto str : text | stdv::split(',') | stdv::transform(make<std::string_view>))
    {
        sum += Hash(str);
    }
    return sum;
}

size_t CalcPower(const std::array<std::vector<Lens>, 256>& boxes)
{
    size_t power = 0;
    for (auto [box_idx, box] : boxes | stdv::enumerate) {
        for (auto [lens_idx, lens] : boxes[box_idx] | stdv::enumerate) {
            power += (box_idx + 1) * (lens_idx + 1) * lens.fl;
        }
    }
    return power;
}

size_t Solve_2(const std::filesystem::path& input)
{
    std::array<std::vector<Lens>, 256> boxes;

    const auto text = ReadText(input);
    for (auto part : text | stdv::split(',') | stdv::transform(make<std::string_view>))
    {
        auto [lens, op] = ParseEntry(part);

        int box_idx = Hash(lens.label);
        auto& box = boxes[box_idx];

        auto itr = stdr::find_if(box, [lens](Lens l) { return l.label == lens.label; });
        if (op == '-')
        {
            if (itr != box.end()) box.erase(itr);
        }
        if (op == '=')
        {     
            if (itr != box.end()) {
                itr->fl = lens.fl;
            } else {
                box.push_back(lens);
            }         
        }
    }

    return CalcPower(boxes);
}