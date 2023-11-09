export module utils;

export import std;

export import :Math;
export import :Array2D;
export import :RectViews;

export
{
	namespace stdr = std::ranges;
	namespace stdv = std::ranges::views;

	/* File IO */
	std::string ReadText(const std::filesystem::path& filename);
	std::vector<std::string> ReadLines(const std::filesystem::path& filename);

	/* Parsing */
	int ReadInt(std::string_view& str, int def = 0);
	std::string_view ReadWord(std::string_view& str, const char* delims = " ,.;");
	bool Skip(std::string_view& str, std::string_view prefix);

	/* Predicates */
	struct pred_not_empty {
		constexpr bool operator() (auto&& v) { return !v.empty(); }
	};
	struct pred_is_empty {
		constexpr bool operator() (auto&& v) { return v.empty(); }
	};
	struct to_int_fn {
		constexpr int operator() (auto&& v) {
			std::string_view str(v);
			int value = def;
			std::from_chars(str.data(), str.data() + str.size(), value);
			return value;
		}
		int def = 0;
	};
	inline constexpr pred_not_empty not_empty;
	inline constexpr pred_is_empty is_empty;
	inline constexpr to_int_fn to_int;

	/* Directions */
	enum class Dir : char {
		None  = 0,
		Left  = 1,
		Right = 2,
		Up    = 3,
		Down  = 4
	};

	constexpr Point DirToOffset(Dir dir) {
		constexpr Point Offsets[] = { {0,0}, {-1,0}, {1,0}, {0,1}, {0,-1} };
		return Offsets[std::to_underlying(dir)];
	}
}