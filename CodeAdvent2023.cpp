import AoC_2023.Day22;

import std;

int main()
{
    const auto res1 = Solve_1("2023/input_22.txt");
    const auto res2 = Solve_2("2023/input_22.txt");

    std::cout << std::format("\nPart 1: {}\n", res1);
    std::cout << std::format("\nPart 2: {}\n", res2);
    return 0;
}