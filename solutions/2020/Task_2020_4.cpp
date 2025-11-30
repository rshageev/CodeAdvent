#include "Runner.h"

import std;
import utils;

namespace
{
    using Passport = std::map<std::string, std::string>;

    auto ParseLine(std::string_view line, Passport& pass)
    {
        for (auto str : Split(line, " ")) {
            auto key = ReadWord(str, ":");
            Skip(str, ":");
            pass.emplace(key, str);
        }
    }

    auto LoadData(const std::filesystem::path& input)
    {
        std::vector<Passport> passports;
        for (const auto& block : ReadLineBlocks(input)) {
            auto& pass = passports.emplace_back();
            for (const auto& line : block) {
                ParseLine(line, pass);
            }
        }
        return passports;
    }

    bool AllFieldsArePresent(const Passport& pass)
    {
        const std::string req_fields[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };

        return stdr::all_of(req_fields, [&](const auto& field) { return pass.contains(field); });
    }

    bool CheckYear(std::string_view str, int min, int max)
    {
        const int year = to_int(str);
        return (year >= min && year <= max);
    }

    bool CheckHeight(std::string_view str)
    {
        int value = Read<int>(str);
        if (str == "cm") {
            return value >= 150 && value <= 193;
        }
        if (str == "in") {
            return value >= 59 && value <= 76;
        }
        return false;
    }

    bool CheckHairColor(std::string_view str)
    {
        if (str[0] != '#') return false;

        str.remove_prefix(1);
        return stdr::all_of(str, [](char ch) {
            return (ch >= '0' && ch <= '9')
                || (ch >= 'a' && ch <= 'f');
            });
    }

    bool CheckEyeColor(std::string_view str)
    {
        const std::string_view valid[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        return contains(valid, str);
    }

    bool CheckPID(std::string_view pid)
    {
        return pid.size() == 9 && stdr::all_of(pid, is_digit);
    }

    bool AllFieldsAreValid(const Passport& pass)
    {
        return AllFieldsArePresent(pass)
            && CheckYear(pass.at("byr"), 1920, 2002)
            && CheckYear(pass.at("iyr"), 2010, 2020)
            && CheckYear(pass.at("eyr"), 2020, 2030)
            && CheckHeight(pass.at("hgt"))
            && CheckHairColor(pass.at("hcl"))
            && CheckEyeColor(pass.at("ecl"))
            && CheckPID(pass.at("pid"));
    }

    auto Solve_1(const std::filesystem::path& input)
    {
        auto passports = LoadData(input);
        return stdr::count_if(passports, AllFieldsArePresent);
    }

    auto Solve_2(const std::filesystem::path& input)
    {
        auto passports = LoadData(input);
        return stdr::count_if(passports, AllFieldsAreValid);
    }

    REGISTER_SOLUTION(2020, 4, 1, Solve_1);
    REGISTER_SOLUTION(2020, 4, 2, Solve_2);
}
