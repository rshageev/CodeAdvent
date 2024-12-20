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