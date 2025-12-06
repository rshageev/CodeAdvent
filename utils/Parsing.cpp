module utils;

import std;

std::string_view ReadWord(std::string_view& str, const char* delims)
{
	const auto pos = str.find_first_of(delims, 0);
	const auto result = str.substr(0, pos);
	str.remove_prefix(std::min(pos, str.size()));
	return result;
}

char ReadChar(std::string_view& str)
{
	char ch = str[0];
	str.remove_prefix(1);
	return ch;
}

bool Skip(std::string_view& str, std::string_view prefix)
{
	if (!str.starts_with(prefix)) {
		return false;
	}
	str.remove_prefix(prefix.size());
	return true;
}

std::vector<std::string_view> Split(std::string_view str, std::string_view delim, bool repeat_delim)
{
	std::vector<std::string_view> parts;

	auto pos = str.find(delim);
	while (pos != std::string_view::npos) {
		parts.push_back(str.substr(0, pos));
		str.remove_prefix(pos + delim.size());
		while (repeat_delim && str.starts_with(delim)) {
			str.remove_prefix(delim.size());
		}
		pos = str.find(delim);
	}

	parts.push_back(str);

	return parts;
}
