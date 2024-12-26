#include "Runner.h"

import std;
import utils;

namespace
{
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

	REGISTER_SOLUTION(2015, 1, 1, Solve_1);
	REGISTER_SOLUTION(2015, 1, 2, Solve_2);
}
