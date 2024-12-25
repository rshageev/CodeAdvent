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

	void Run(int year, int day, std::string_view name)
	{
		auto& all_solutions = GetSolutions();
		const auto& day_solutions = all_solutions[{ year, day }];
		for (auto& [func, p, id] : day_solutions) {
			if (id == name) {
				std::cout << std::format("Part {}\n", p);
				// run test
				std::string test_file = std::format("{}/day{}/test.txt", year, day);
				std::string test_result = func(test_file, true);
				std::cout << std::format("Test result: {}\n", test_result);
				// run input
				std::string input_file = std::format("{}/day{}/input.txt", year, day);
				std::string result = func(input_file, false);
				std::cout << std::format("Result: {}\n", result);
			}
		}
	}
}