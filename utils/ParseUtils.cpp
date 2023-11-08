module utils;

import std;

std::optional<int> ToInt(std::string_view str)
{
	int value = 0;
	const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, 10);
	if (ec == std::errc{}) {
		return { value };
	}
	return {};
}

std::tuple<int, std::string_view> ReadInt(std::string_view str, int def)
{
	int value = def;
	const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, 10);
	return { value, str.substr(ptr - str.data()) };
}