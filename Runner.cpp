#include "Runner.h"

import std;

namespace Solutions
{
	static constexpr std::string_view ClrReset = "\033[0m";
	static constexpr std::string_view ClrBlack = "\033[30m";
	static constexpr std::string_view ClrRed = "\033[31m";
	static constexpr std::string_view ClrGreen = "\033[32m";
	static constexpr std::string_view ClrYellow = "\033[33m";
	static constexpr std::string_view ClrBlue = "\033[34m";
	static constexpr std::string_view ClrPurple = "\033[35m";
	static constexpr std::string_view ClrCyan = "\033[36m";
	static constexpr std::string_view ClrWhite = "\033[37m";

	static constexpr std::string_view ItalicOn = "\033[3m";
	static constexpr std::string_view ItalicOff = "\033[23m";

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

	using Answers = std::array<std::map<std::string, std::string>, 2>;

	Answers LoadAnswers(Date date)
	{
		Answers res;
		res[0]["input"] = "";
		res[1]["input"] = "";

		auto filename = std::format("{}/day{}/answers.txt", date.year, date.day);
		if (std::filesystem::exists(filename)) {
			for (std::string_view line : ReadLines(filename)) {
				auto input_name = ReadWord(line); Skip(line, " ");
				int part = Read<int>(line); Skip(line, " ");

				res[part - 1][std::string(input_name)] = std::string(line);
			}
		}

		return res;
	}

	void Run(Date date, const Solution& sln, const Answers& answers)
	{
		std::cout << ItalicOn;
		if (sln.name.empty()) {
			std::cout << std::format("Part {}\n", sln.part);
		} else {
			std::cout << std::format("Part {} ({})\n", sln.part, sln.name);
		}
		std::cout << ItalicOff;

		// Run solution on all described inputs
		for (const auto& [in, res] : answers[sln.part - 1])
		{
			std::string input_file = std::format("{}/day{}/{}.txt", date.year, date.day, in);
			if (std::filesystem::exists(input_file)) {
				auto [result, time] = sln.func(input_file);

				std::string_view status;
				if (!res.empty()) {
					status = (result == res) ? "\033[32mOK\033[0m" : "\033[31mERROR\033[0m";
				}
				std::cout << std::format("  {:<12.12s} {} {}\n", in + ":", result, status);
			}
			else {
				std::cout << "  {}:\t <file is missing>\n";
			}
		}
	}

	void RunAll()
	{
		auto& all_solutions = GetSolutions();
		for (const auto& [date, day_solutions] : all_solutions)
		{
			auto answers = LoadAnswers(date);
			std::cout << ClrCyan << std::format("= Day {}, Year {} =\n", date.day, date.year) << ClrReset;
			for (auto& sln : day_solutions) {
				Run(date, sln, answers);
			}
			std::cout << '\n';
		}
	}

	void Run(int year)
	{
		auto& all_solutions = GetSolutions();
		for (const auto& [date, day_solutions] : all_solutions) {
			if (date.year == year) {
				auto answers = LoadAnswers(date);
				std::cout << ClrCyan << std::format("= Day {}, Year {} =\n", date.day, date.year) << ClrReset;
				for (auto& sln : day_solutions) {
					Run(date, sln, answers);
				}
				std::cout << '\n';
			}
		}
	}

	void Run(int year, int day)
	{
		Date date{ year, day };
		std::cout << std::format("\033[36m= Day {}, Year {} =\n\033[0m", day, year);
		auto& all_solutions = GetSolutions();
		const auto& day_solutions = all_solutions[date];
		const auto answers = LoadAnswers(date);
		for (auto& sln : day_solutions) {
			Run(date, sln, answers);
		}
		std::cout << '\n';
	}


	void Profile(int year)
	{
		Timer::Duration total_time = Timer::Duration::zero();

		for (const auto& [date, day_solutions] : GetSolutions())
		{
			if (date.year == year)
			{
				std::string input_file = std::format("{}/day{}/input.txt", date.year, date.day);

				Timer::Duration best_time[2] = { Timer::Duration::max(), Timer::Duration::max() };

				for (auto& sln : day_solutions) {
					auto [result, time] = sln.func(input_file);
					best_time[sln.part - 1] = std::min(best_time[sln.part - 1], time);
				}

				std::string time_str[2];
				for (int i = 0; i < 2; ++i) {
					if (best_time[i] < Timer::Duration::max()) {
						double msd = std::chrono::duration<double, std::milli>(best_time[i]).count();
						time_str[i] = std::format("{:.3f} ms", msd);
						total_time += best_time[i];
					}
				}

				std::cout << std::format("{:>2} : {:>12} {:>12}\n", date.day, time_str[0], time_str[1]);
			}
		}

		std::chrono::duration<double, std::milli> total_time_ms = total_time;
		std::cout << std::format("\033[1mTotal time: {}\033[0m\n", total_time_ms);
	}
}