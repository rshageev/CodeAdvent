
import AoC_2024.Day8;

import std;

int main()
{
    const auto res1t = Solve_1("test.txt");
    const auto res1 = Solve_1("input.txt");
    const auto res2t = Solve_2("test.txt");
    const auto res2 = Solve_2("input.txt");

    std::cout << std::format("\nPart 1: test {}, answer {}\n", res1t, res1);
    std::cout << std::format("\nPart 2: test {}, answer {}\n", res2t, res2);
    return 0;
}