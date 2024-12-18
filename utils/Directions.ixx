export module utils:Directions;

import std;

import :Math;

export
{
	using Direction = std::uint8_t;
	namespace Dir {
		inline constexpr Direction None  = 0;

		inline constexpr Direction Right = 0b0000'0001;
		inline constexpr Direction Up    = 0b0000'0100;
		inline constexpr Direction Left  = 0b0001'0000;
		inline constexpr Direction Down  = 0b0100'0000;

		inline constexpr Direction E  = 0b0000'0001;
		inline constexpr Direction NE = 0b0000'0010;
		inline constexpr Direction N  = 0b0000'0100;
		inline constexpr Direction NW = 0b0000'1000;
		inline constexpr Direction W  = 0b0001'0000;
		inline constexpr Direction SW = 0b0010'0000;
		inline constexpr Direction S  = 0b0100'0000;
		inline constexpr Direction SE = 0b1000'0000;
	}

	inline constexpr std::pair<Direction, Point> Neighbours4[] = {
		{ Dir::E, { 1, 0 } },
		{ Dir::N, { 0, 1 } },
		{ Dir::W, { -1, 0 } },
		{ Dir::S, { 0, -1 } },
	};

	inline constexpr std::pair<Direction, Point> Neighbours8[] = {
		{ Dir::E, { 1, 0 } },
		{ Dir::NE, { 1, 1 } },
		{ Dir::N, { 0, 1 } },
		{ Dir::NW, { -1, 1 } },
		{ Dir::W, { -1, 0 } },
		{ Dir::SW, { -1, -1 } },
		{ Dir::S, { 0, -1 } },
		{ Dir::SE, { 1, -1 } },
	};

	template<std::integral T = int>
	constexpr TPoint<T> DirToOffset(Direction dir) {
		for (auto [d, off] : Neighbours8) {
			if (d == dir) return { off.x, off.y };
		}
		return { 0, 0 };
	}

	template<std::integral T = int>
	constexpr TPoint<T> MovePoint(TPoint<T> pt, Direction dir, T dist = 1) {
		return pt + DirToOffset<T>(dir) * dist;
	}

	constexpr bool IsVertical(Direction dir) {
		return dir & (Dir::Up | Dir::Down);
	}
	constexpr bool IsHorizontal(Direction dir) {
		return dir & (Dir::Left | Dir::Right);
	}
	constexpr Direction RotateRight(Direction dir) {
		return std::rotr(dir, 2);
	}
	constexpr Direction RotateLeft(Direction dir) {
		return std::rotl(dir, 2);
	}
	constexpr Direction RotateRight45(Direction dir) {
		return std::rotr(dir, 1);
	}
	constexpr Direction RotateLeft45(Direction dir) {
		return std::rotl(dir, 1);
	}
	constexpr Direction Opposite(Direction dir) {
		return std::rotl(dir, 4);
	}

	struct char_to_dir
	{
		std::string_view table = "<>^v";

		constexpr Direction operator() (char ch) const
		{
			if (ch == table[0]) return Dir::Left;
			if (ch == table[1]) return Dir::Right;
			if (ch == table[2]) return Dir::Up;
			if (ch == table[3]) return Dir::Down;
			return Dir::None;
		}
	};

	Direction CharToDir(char ch, std::string_view table = "<>^v")
	{
		return char_to_dir{ table }(ch);
	}

	char DirToChar(Direction dir)
	{
		switch (dir) {
			case Dir::None: return '.';
			case Dir::Left: return '<';
			case Dir::Right: return '>';
			case Dir::Up: return '^';
			case Dir::Down: return 'v';
		}
		return '0' + std::popcount(dir);
	}
}