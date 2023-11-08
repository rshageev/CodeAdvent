export module utils;

import std;

export import :math;

export
{
	/* File IO */
	std::vector<std::string> ReadLines(const std::filesystem::path& file);

	/* Parsing */
	std::optional<int> ToInt(std::string_view str);
	std::tuple<int, std::string_view> ReadInt(std::string_view str, int def = 0);
}