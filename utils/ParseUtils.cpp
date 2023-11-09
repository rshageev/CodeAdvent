module utils;

import std;

int ReadInt(std::string_view& str, int def)
{
	int value = def;
	const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, 10);
	str.remove_prefix(ptr - str.data());
	return value;
}

std::string_view ReadWord(std::string_view& str, const char* delims)
{
	const auto pos = str.find_first_of(delims, 0);
	const auto result = str.substr(0, pos);
	str.remove_prefix(std::min(pos, str.size()));
	return result;
}

bool Skip(std::string_view& str, std::string_view prefix)
{
	if (!str.starts_with(prefix)) {
		//std::cout << "Parse error\n";
		return false;
	}
	str.remove_prefix(prefix.size());
	return true;
}