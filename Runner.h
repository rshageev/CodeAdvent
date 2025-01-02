#pragma once

import std;
import utils;

namespace Solutions
{
	using SolutionFunc = std::function<std::pair<std::string, Timer::Duration>(const std::filesystem::path&)>;

	struct Registrator
	{
		Registrator(int year, int day, int part, SolutionFunc func, std::string name = "");
	};

	void Run(int year, int day);
	void Run(int year);
	void RunAll();

	void Profile(int year);

	
	template<typename Func>
	requires std::invocable<Func, const std::filesystem::path&>
	SolutionFunc MakeFunc(Func&& func) {
		return [func](const std::filesystem::path& input) {
			Timer timer;
			const auto result = std::invoke(func, input);
			const auto duration = timer.Get();
			return std::pair(std::format("{}", result), duration);
		};
	}
}

#define GEN_UNIQUE_NAME_COUNTER2( name, counter ) name##counter
#define GEN_UNIQUE_NAME_COUNTER( name, line ) GEN_UNIQUE_NAME_COUNTER2( name, line )
#define GEN_UNIQUE_NAME(name) GEN_UNIQUE_NAME_COUNTER(name, __LINE__)

#define REGISTER_SOLUTION(year, day, part, func, ...) \
	namespace { const Solutions::Registrator GEN_UNIQUE_NAME(autoReg) (year, day, part, Solutions::MakeFunc(func), __VA_ARGS__); }
