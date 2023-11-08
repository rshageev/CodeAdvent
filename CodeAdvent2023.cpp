import utils;

import std;

namespace stdr = std::ranges;
namespace stdv = std::ranges::views;

int main()
{
    for (const auto& line : ReadLines("input_10.txt") | stdv::filter(not_empty))
    {
        std::cout << line << '\n';
    }
}