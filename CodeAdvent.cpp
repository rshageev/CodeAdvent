
import AoC_2024.Day22;

import std;
import utils;

int main()
{
    const auto res1t = Solve_1("test.txt");
    const auto res2t = Solve_2("test.txt");

    Timer timer;
    const auto res1 = Solve_1("input.txt");
    const auto time1 = timer.Get();
    const auto res2 = Solve_2("input.txt");
    const auto time2 = timer.Get();

    std::cout << std::format("\nPart 1: test {}, answer {}, time {}\n", res1t, res1, time1);
    std::cout << std::format("\nPart 2: test {}, answer {}, time {}\n", res2t, res2, time2);
    return 0;
}