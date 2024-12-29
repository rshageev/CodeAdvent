module;

#include <Eigen/Dense>

module utils;

namespace
{
    // Finds x, such as (a * x) % m == 1
    int64 inv_mod(int64 a, int64 m)
    {
        int64 m0 = m, t, q;
        int64 x0 = 0, x1 = 1;

        if (m == 1)
            return 0;

        while (a > 1) {
            q = a / m;
            t = m;
            m = a % m, a = t;
            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }

        if (x1 < 0)
            x1 += m0;

        return x1;
    }
}

/*
* Chinese remainder theorem
*
* implementation from:
* https://www.geeksforgeeks.org/implementation-of-chinese-remainder-theorem-inverse-modulo-based-implementation/
*/
int64 SolveCRT(std::span<const int64> numbers, std::span<const int64> remainders)
{
    int64 prod = 1;
    for (auto n : numbers)
        prod *= n;

    int64 result = 0;
    for (auto [num, rem] : std::views::zip(numbers, remainders)) {
        int64 pp = prod / num;
        result += rem * inv_mod(pp, num) * pp;
    }

    return result % prod;
}

std::vector<double> SolveSystemEigen(const std::vector<std::vector<double>>& equations)
{
    size_t size = equations.size();

    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> coeff_mat(size, size);
    Eigen::Vector<double, Eigen::Dynamic> res_vec(size);

    for (const auto& [y, eq] : equations | stdv::enumerate) {
        for (size_t x = 0; x < size; ++x) {
            coeff_mat(y, x) = eq[x];
        }
        res_vec(y) = eq.back();
    }

    Eigen::Vector<double, Eigen::Dynamic> sln = coeff_mat.fullPivLu().solve(res_vec);

    return { sln.data(), sln.data() + sln.size() };
}