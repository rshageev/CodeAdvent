#include "Runner.h"

import std;

namespace Solutions
{
	struct Date
	{
		int year;
		int day;

		auto operator<=>(const Date&) const = default;
	};

	struct Solution
	{
		SolutionFunc func;
		int part;
		std::string name;
	};

	using AllSolutions = std::map<Date, std::vector<Solution>>;

	static std::unique_ptr<AllSolutions> all_solutions;

	AllSolutions& GetSolutions() {
		if (!all_solutions) {
			all_solutions = std::make_unique<AllSolutions>();
		}
		return *all_solutions;
	}

	Registrator::Registrator(int year, int day, int part, SolutionFunc func, std::string name)
	{
		auto& all_solutions = GetSolutions();
		auto& day_solutions = all_solutions[{ year, day }];
		day_solutions.emplace_back(std::move(func), part, std::move(name));
	}

	void Run(Date date, const Solution& sln)
	{
		if (sln.name.empty()) {
			std::cout << std::format("\033[32mPart {}\n\033[0m", sln.part);
		} else {
			std::cout << std::format("\033[32mPart {} ({})\n\033[0m", sln.part, sln.name);
		}
		// run test
		std::string test_file = std::format("{}/day{}/test.txt", date.year, date.day);
		std::string test_result = sln.func(test_file);
		std::cout << std::format("  Test:\t\t {}\n", test_result);
		// run input
		std::string input_file = std::format("{}/day{}/input.txt", date.year, date.day);
		std::string result = sln.func(input_file);
		std::cout << std::format("  Result:\t {}\n", result);
	}

	void RunAll()
	{
		auto& all_solutions = GetSolutions();
		for (const auto& [date, day_solutions] : all_solutions) {
			std::cout << std::format("\033[36m= Day {}, Year {} =\n\033[0m", date.day, date.year);
			for (auto& sln : day_solutions) {
				Run(date, sln);
			}
			std::cout << '\n';
		}
	}

	void Run(int year, int day)
	{
		std::cout << std::format("\033[36m= Day {}, Year {} =\n\033[0m", day, year);
		auto& all_solutions = GetSolutions();
		const auto& day_solutions = all_solutions[{ year, day }];
		for (auto& sln : day_solutions) {
			Run(Date{ year, day }, sln);
		}
		std::cout << '\n';
	}
}