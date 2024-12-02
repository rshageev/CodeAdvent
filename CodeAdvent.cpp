
import AoC_2024.Day1;

import std;

int main()
{
    const auto res1 = Solve_1("input.txt");
    const auto res2 = Solve_2("input.txt");

    std::cout << std::format("\nPart 1: {}\n", res1);
    std::cout << std::format("\nPart 2: {}\n", res2);
    return 0;
}