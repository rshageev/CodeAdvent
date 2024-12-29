export module utils:Algo;

import std;

import :Types;

export
{
    // solves chinese remainder theorem
    int64 SolveCRT(std::span<const int64> numbers, std::span<const int64> remainders);

    // solve a linear equation system using Eigen library
    std::vector<double> SolveSystemEigen(const std::vector<std::vector<double>>& equations);

    // solve a linear equations system using Gaussian elimination
    std::vector<double> SolveSystemGauss(const std::vector<std::vector<double>>& equations);
}
