#include "Runner.h"

import std;
import utils;

namespace
{
    auto ReadData(const std::filesystem::path& input)
    {
        std::vector<std::pair<uint64, uint64>> ids;
        auto text = ReadText(input);
        for (auto id : Split(text, ",")) {
            const auto pos = id.find('-');
            auto first = to_uint64(id.substr(0, pos));
            auto second = to_uint64(id.substr(pos + 1));
            ids.emplace_back(first, second);
        }
        return ids;
    }

    bool IsInvalid1(std::string_view id)
    {
        auto sz = id.size() / 2;
        return id.substr(0, sz) == id.substr(sz);
    }

    bool IsDividedIntoNEqualParts(std::string_view id, size_t div)
    {   
        if (id.size() % div != 0)
            return false;

        size_t sz = id.size() / div;
        auto ss = id.substr(0, sz);
        for (size_t i = 1; i < div; ++i) {
            if (id.substr(i * sz, sz) != ss) {
                return false;
            }
        }
        return true;
    }

    bool IsInvalid2(std::string_view id)
    {
        for (size_t div = 2; div <= id.size(); ++div) {
            if (IsDividedIntoNEqualParts(id, div))
                return true;
        }
        return false;
    }

    using Pred = bool (*)(std::string_view);
    auto Solve(Pred is_invalid, const std::filesystem::path& input)
    {
        uint64 sum = 0;
        for (auto [from, to] : ReadData(input)) {
            for (auto id = from; id <= to; ++id) {
                if (is_invalid(std::to_string(id))) {
                    sum += id;
                }
            }
        }
        return sum;
    }

    REGISTER_SOLUTION(2025, 2, 1, std::bind_front(Solve, IsInvalid1));
    REGISTER_SOLUTION(2025, 2, 2, std::bind_front(Solve, IsInvalid2));
}
