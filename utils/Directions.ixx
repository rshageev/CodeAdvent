export module utils:Directions;

import std;

import :Math;

export
{
	using Direction = std::uint8_t;
	namespace Dir {
		inline constexpr Direction None  = 0;
		inline constexpr Direction Right = 0b0000'0001; // 1
		inline constexpr Direction Up    = 0b0000'0100; // 4
		inline constexpr Direction Left  = 0b0001'0000; // 16
		inline constexpr Direction Down  = 0b0100'0000; // 64
	}

	inline constexpr std::pair<Direction, Point> Neighbours4[] = {
		{ Dir::Right, { 1, 0 } },
		{ Dir::Up, { 0, 1 } },
		{ Dir::Left, { -1, 0 } },
		{ Dir::Down, { 0, -1 } },
	};

	constexpr Point DirToOffset(Direction dir) {
		switch (dir) {
			case Dir::Right: return { 1, 0 };
			case Dir::Up: return { 0, 1 };
			case Dir::Left: return { -1, 0 };
			case Dir::Down: return { 0, -1 };
			default: return { 0, 0 };
		}
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
	constexpr Direction RotateRight(Direction dir) {
		return std::rotr(dir, 2);
	}
	constexpr Direction RotateLeft(Direction dir) {
		return std::rotl(dir, 2);
	}
	constexpr Direction Opposite(Direction dir) {
		return std::rotl(dir, 4);
	}

	
	Direction CharToDir(char ch)
	{
		switch (ch) {
			case '<': return Dir::Left;
			case '>': return Dir::Right;
			case '^': return Dir::Up;
			case 'v': return Dir::Down;
			default: return Dir::None;
		}
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