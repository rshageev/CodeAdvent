export module utils:Regex;

import std;

export
{
	auto RegexSearch(const std::string& str, const std::regex& regex)
	{
		std::sregex_iterator first(str.begin(), str.end(), regex);
		std::sregex_iterator last;
		return std::ranges::subrange(first, last);
	}
}