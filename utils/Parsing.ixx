export module utils:Parsing;

import std;

export
{
	template<std::integral T, int base = 10>
	T Read(std::string_view& str, T def = 0) {
		T value = def;
		const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, base);
		str.remove_prefix(ptr - str.data());
		return value;
	}

	template<std::integral T, int base = 10>
	T Read(std::string_view&& str, T def = 0) {
		T value = def;
		std::from_chars(str.data(), str.data() + str.size(), value, base);
		return value;
	}

	std::string_view ReadWord(std::string_view& str, const char* delims = " ,.;");
	char ReadChar(std::string_view& str);
	bool Skip(std::string_view& str, std::string_view prefix);

	std::vector<std::string_view> Split(std::string_view str, std::string_view delim, bool repeat_delim = false);

	constexpr std::pair<std::string_view, std::string_view> Split2(std::string_view str, std::string_view delim) {
		auto pos = str.find(delim);
		return { str.substr(0, pos), str.substr(pos + delim.size()) };
	}

	constexpr std::pair<std::string_view, std::string_view> Split2(std::string_view str, char delim) {
		auto pos = str.find(delim);
		return { str.substr(0, pos), str.substr(pos + 1) };
	}

	struct Parse {
		std::string_view str;
		bool success = true;

		template<std::integral T, int base = 10>
		Parse& Read(T& out) {
			if (success) {
				const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), out, base);
				str.remove_prefix(ptr - str.data());
				if (ec != std::errc{}) success = false;
			}
			return *this;
		}

		Parse& Skip(std::string_view prefix) {
			success = success && ::Skip(str, prefix);
			return *this;
		}

		Parse& ReadWord(std::string_view& out, const char* delims = " ,.;") {
			if (success) {
				out = ::ReadWord(str, delims);
			}
		}
	};
}