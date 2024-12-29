export module utils:Algo;

import std;

import :Types;

export
{
    int64 SolveCRT(std::span<const int64> numbers, std::span<const int64> remainders);

    std::vector<double> SolveSystemEigen(const std::vector<std::vector<double>>& equations);
}
