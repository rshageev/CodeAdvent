export module utils:math;

import std;

export
{
	struct Point
	{
		int x = 0;
		int y = 0;

		constexpr auto operator<=>(const Point& rhs) const = default;

		Point& operator+=(const Point& rhs) { x += rhs.x; y += rhs.y; return *this; }
	};

	constexpr Point operator+ (Point p1, Point p2) { return { p1.x + p2.x, p1.y + p2.y }; }
	constexpr Point operator- (Point p1, Point p2) { return { p1.x + p2.x, p1.y + p2.y }; }
}