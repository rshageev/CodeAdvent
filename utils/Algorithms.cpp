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

int64 SolveCRT(std::span<const int64> numbers, std::span<const int64> remainders)
{
    // https://www.geeksforgeeks.org/implementation-of-chinese-remainder-theorem-inverse-modulo-based-implementation/

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

std::vector<double> SolveSystemGauss(const std::vector<std::vector<double>>& equations)
{
    auto mat = equations;
    const int N = static_cast<int>(equations.size());

    for (int i = 0; i < N; ++i)
    {
        auto& cnt_row = mat[i];

        int imax;
        double vmax = 0.0;
        for (int j = i; j < N; j++) {
            if (auto v = std::abs(mat[j][i]); v > vmax) {
                vmax = v;
                imax = j;
            }
        }

        if (vmax <= 0.00001) {
            return {};
        }

        std::swap(cnt_row, mat[imax]);

        for (int j = i + 1; j < N; j++) {
            auto& row = mat[j];
            auto r = row[i] / cnt_row[i];
            for (int k = i; k < N + 1; k++) {
                row[k] -= r * cnt_row[k];
            }
        }
    }

    std::vector<double> res(mat.size());
    for (int i = N - 1; i >= 0; i--) {
        res[i] = mat[i].back();
        for (int j = i + 1; j < N; j++)
            res[i] -= mat[i][j] * res[j];
        res[i] /= mat[i][i];
    }

    return res;
}
