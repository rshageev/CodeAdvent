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

	constexpr bool is_digit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	template<size_t N>
	struct tuple_get_fn {
		constexpr auto operator() (auto&& t) { return std::get<N>(t); }
	};
	template<size_t N>
	inline constexpr tuple_get_fn<N> tuple_get;

	template<auto V>
	struct equal_fn {
		constexpr bool operator() (auto&& v) const { return v == V; }
	};
	template<auto V>
	inline constexpr equal_fn<V> equal;

	template<class P>
	struct apply_to_pair {
		P pred;
		auto operator() (auto&& p) {
			auto& [p1, p2] = p;
			return std::invoke(pred, p1, p2);
		}
	};

	template<class P>
	struct apply_to_pair_rev {
		P pred;
		auto operator() (auto&& p) {
			auto& [p1, p2] = p;
			return std::invoke(pred, p2, p1);
		}
	};

	constexpr bool contains(const auto& range, const auto& v) {
		return stdr::find(range, v) != stdr::end(range);
	}

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