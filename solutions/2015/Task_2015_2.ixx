export module AoC_2015.Day2;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export int Solve_2(const std::filesystem::path& input);

module : private;

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
	return Box{ dim[0], dim[1], dim[2]};
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
