#pragma once

import std;

namespace Solutions
{
	using SolutionFunc = std::function<std::string(const std::filesystem::path&, bool)>;

	struct Registrator
	{
		Registrator(int year, int day, int part, SolutionFunc func, std::string name = "default");
	};

	void Run(int year, int day, std::string_view name = "default");

	template<typename Func>
	requires std::invocable<Func, const std::filesystem::path>
	SolutionFunc MakeFunc(Func&& func) {
		return [func](const std::filesystem::path& input, bool) {
			return std::to_string(func(input));
		};
	}
}

#define GEN_UNIQUE_NAME_COUNTER2( name, counter ) name##counter
#define GEN_UNIQUE_NAME_COUNTER( name, line ) GEN_UNIQUE_NAME_COUNTER2( name, line )
#define GEN_UNIQUE_NAME(name) GEN_UNIQUE_NAME_COUNTER(name, __LINE__)

#define REGISTER_SOLUTION(year, day, part, func) \
	namespace { const Solutions::Registrator GEN_UNIQUE_NAME(autoReg) (year, day, part, Solutions::MakeFunc(func)); }
