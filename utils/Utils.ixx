export module utils;

export import std;

export import :Math;
export import :Array2D;
export import :RectViews;
export import :Timer;
export import :Functional;
export import :Directions;
export import :Types;

export
{
	namespace stdr = std::ranges;
	namespace stdv = std::ranges::views;

	/* File IO */
	std::string ReadText(const std::filesystem::path& filename);
	std::vector<std::string> ReadLines(const std::filesystem::path& filename, bool remove_last_empty = true);
	std::vector<std::vector<std::string>> ReadLineBlocks(const std::filesystem::path& filename);

	template<class Pred = std::identity>
	auto ReadArray2D(const std::filesystem::path& filename, Pred&& pred = {}) {
		return Array2DFromString(ReadText(filename), std::forward<Pred>(pred));
	}

	/* Parsing */
	
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

	/* Hashing */

	constexpr size_t HashCombine(size_t hash1, size_t hash2)
	{
		return hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
	}

	struct PairHash {
		template<class T1, class T2>
		size_t operator() (const std::pair<T1, T2>& p) const {
			size_t h1 = std::hash<T1>{}(p.first);
			size_t h2 = std::hash<T2>{}(p.second);
			return HashCombine(h1, h2);
		}
	};
}
