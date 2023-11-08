export module AoC_2015.Day1;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

int Solve_1(const std::filesystem::path& input)
{
	const auto data = ReadText(input);
	return static_cast<int>(stdr::count(data, '(')) - static_cast<int>(stdr::count(data, ')'));
}

int Solve_2(const std::filesystem::path& input)
{
	const auto data = ReadText(input);

	int floor = 0;
	int pos = 0;
	for (char ch : data) {
		++pos;
		if (ch == '(') ++floor;
		if (ch == ')') --floor;
		if (floor < 0) {
			return pos;
		}
	}
	return pos;
}
