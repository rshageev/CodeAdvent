#include <nlohmann/json.hpp>
#include "Runner.h"

import std;
import utils;

namespace
{
    int CountNumbers(const nlohmann::json& data)
    {
        int result = 0;
        if (data.is_number())
        {
            result = data.get<int>();
        }
        else if (data.is_array())
        {
            for (const auto& elem : data) {
                result += CountNumbers(elem);
            }
        }
        else if (data.is_object())
        {
            for (const auto& [key, value] : data.items())
            {
                if (value == "red") {
                    return 0;
                } else {
                    result += CountNumbers(value);
                }
            }
        }
        return result;
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        const std::string text = ReadText(input);
        const std::regex regex(R"(-?\d+)");
        int res = 0;
        for (std::smatch match : RegexSearch(text, regex)) {
            res += to_int(match);
        }
        return res;
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        const std::string text = ReadText(input);
        auto data = nlohmann::json::parse(text);
        return CountNumbers(data);
    }

    REGISTER_SOLUTION(2015, 12, 1, Solve_1);
    REGISTER_SOLUTION(2015, 12, 2, Solve_2);
}
