#include "Runner.h"

import std;
import utils;

namespace
{
	struct Box {
		int w, h, l;
	};

	constexpr int PaperForBox(Box b)
	{
		const int s1 = b.w * b.h;
		const int s2 = b.w * b.l;
		const int s3 = b.h * b.l;
		return 2 * (s1 + s2 + s3) + std::min({ s1, s2, s3 });
	}

	constexpr int RibbonForBox(Box b)
	{
		const int d1 = b.w + b.l;
		const int d2 = b.w + b.h;
		const int d3 = b.h + b.l;
		const int d = 2 * std::min({ d1, d2, d3 });

		return d + b.w * b.h * b.l;
	}

	static_assert(RibbonForBox({ 2, 3, 4 }) == 34);

	Box ParseLine(std::string_view str)
	{
		const auto dim = str | stdv::split('x') | stdv::transform(to_int) | stdr::to<std::vector>();
		return Box{ dim[0], dim[1], dim[2] };
	}

	int Solve_1(const std::filesystem::path& input)
	{
		int total = 0;
		for (const auto& line : ReadLines(input))
		{
			total += PaperForBox(ParseLine(line));
		}
		return total;
	}

	int Solve_2(const std::filesystem::path& input)
	{
		int total = 0;
		for (const auto& line : ReadLines(input))
		{
			total += RibbonForBox(ParseLine(line));
		}
		return total;
	}

	REGISTER_SOLUTION(2015, 2, 1, Solve_1);
	REGISTER_SOLUTION(2015, 2, 2, Solve_2);
}
