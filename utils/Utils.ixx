export module utils;

export import std;

export import :Math;
export import :Array2D;
export import :RectViews;
export import :Timer;
export import :Functional;

export
{
	namespace stdr = std::ranges;
	namespace stdv = std::ranges::views;

	/* File IO */
	std::string ReadText(const std::filesystem::path& filename);
	std::vector<std::string> ReadLines(const std::filesystem::path& filename);

	/* Parsing */
	template<std::integral T>
	T Read(std::string_view& str, T def = 0) {
		T value = def;
		const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, 10);
		str.remove_prefix(ptr - str.data());
		return value;
	}
	int ReadInt(std::string_view& str, int def = 0) { return Read<int>(str, def); }
	std::string_view ReadWord(std::string_view& str, const char* delims = " ,.;");
	bool Skip(std::string_view& str, std::string_view prefix);


	/* Directions */
	using Direction = std::uint8_t;
	namespace Dir {
		inline constexpr Direction None = 0;
		inline constexpr Direction Left = 1;
		inline constexpr Direction Right = 2;
		inline constexpr Direction Up = 4;
		inline constexpr Direction Down = 8;
	}

	constexpr Point DirToOffset(Direction dir) {
		constexpr Point Offsets[9] = { {0,0}, {-1,0}, {1,0}, {}, {0,1}, {}, {}, {}, {0,-1} };
		return Offsets[dir];
	}

	constexpr Point MovePoint(Point pt, Direction dir) {
		return pt + DirToOffset(dir);
	}
	constexpr bool IsVertical(Direction dir) {
		return dir & (Dir::Up | Dir::Down);
	}
	constexpr bool IsHorizontal(Direction dir) {
		return dir & (Dir::Left | Dir::Right);
	}

	constexpr Direction Opposite(Direction dir) {
		switch (dir) {
			case Dir::Left: return Dir::Right;
			case Dir::Right: return Dir::Left;
			case Dir::Down: return Dir::Up;
			case Dir::Up: return Dir::Down;
			default: return dir;
		}
	}

	inline constexpr Point Neighbours4[] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
}