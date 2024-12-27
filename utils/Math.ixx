export module utils:Math;

import std;

export
{
	template<class T>
	concept Number = std::integral<T> || std::floating_point<T>;

	template<Number T>
	struct TPoint
	{
		T x = {};
		T y = {};

		constexpr auto operator<=> (const TPoint&) const = default;

		TPoint& operator+= (const TPoint& pt) { x += pt.x; y += pt.y; return *this; }
		TPoint& operator-= (const TPoint& pt) { x -= pt.x; y -= pt.y; return *this; }

		TPoint& operator*= (T v) { x *= v; y *= v; return *this; }
		TPoint& operator/= (T v) { x /= v; y /= v; return *this; }

		constexpr friend TPoint operator+ (TPoint pt1, TPoint pt2) { return { pt1.x + pt2.x, pt1.y + pt2.y }; }
		constexpr friend TPoint operator- (TPoint pt1, TPoint pt2) { return { pt1.x - pt2.x, pt1.y - pt2.y }; }

		constexpr friend TPoint operator* (TPoint pt, T v) { return { pt.x * v, pt.y * v }; }
		constexpr friend TPoint operator* (T v, TPoint pt) { return { pt.x * v, pt.y * v }; }

		constexpr friend TPoint operator/ (TPoint pt, T v) { return { pt.x / v, pt.y / v }; }
	};


	template<Number T>
	struct TRect
	{
		T x = {};
		T y = {};
		T w = {};
		T h = {};

		bool operator== (const TRect&) const = default;
	};

	template<Number T>
	constexpr TPoint<T> LeftBottom(const TRect<T>& rect) { return { rect.x, rect.y }; }

	template<std::integral T>
	constexpr TPoint<T> LeftTop(const TRect<T>& rect) { return { rect.x, rect.y + rect.h - 1 }; }

	template<std::floating_point T>
	constexpr TPoint<T> LeftTop(const TRect<T>& rect) { return { rect.x, rect.y + rect.h }; }

	template<std::integral T>
	constexpr TPoint<T> RightBottom(const TRect<T>& rect) { return { rect.x + rect.w - 1, rect.y }; }

	template<std::floating_point T>
	constexpr TPoint<T> RightBottom(const TRect<T>& rect) { return { rect.x + rect.w, rect.y }; }

	template<std::integral T>
	constexpr TPoint<T> RightTop(const TRect<T>& rect) { return { rect.x + rect.w - 1, rect.y + rect.h - 1 }; }

	template<std::floating_point T>
	constexpr TPoint<T> RightTop(const TRect<T>& rect) { return { rect.x + rect.w, rect.y + rect.h }; }

	template<std::floating_point T>
	constexpr TPoint<T> Center(const TRect<T>& rect) { return { rect.x + rect.w * T(0.5), rect.y + rect.h * T(0.5) }; }


	template<Number T>
	constexpr bool Contains(const TRect<T>& rect, TPoint<T> pt) {
		return pt.x >= rect.x && pt.y >= rect.y && pt.x < rect.x + rect.w && pt.y < rect.y + rect.h;
	}

	template<Number T>
	constexpr TRect<T> Inflated(const TRect<T>& rect, T dx, T dy) {
		return { rect.x - dx, rect.y - dy, rect.w + dx + dx, rect.h + dy + dy };
	}

	template<Number T>
	constexpr TRect<T> BoundingRect(const TRect<T>& rect1, const TRect<T>& rect2) {
		const auto min_x = std::min(rect1.x, rect2.x);
		const auto max_x = std::max(rect1.x + rect1.w, rect2.x + rect2.w);
		const auto min_y = std::min(rect1.y, rect2.y);
		const auto max_y = std::max(rect1.y + rect1.h, rect2.y + rect2.h);
		return { min_x, min_y, max_x - min_x, max_y - min_y };
	}

	template<Number T>
	constexpr T MDist(TPoint<T> p1, TPoint<T> p2) {
		return std::abs(p2.x - p1.x) + std::abs(p2.y - p1.y);
	}

	using Point = TPoint<int>;
	using Pointf = TPoint<float>;
	using Rect = TRect<int>;
	using Rectf = TRect<float>;

	Pointf ToFloat(Point pt)
	{
		return { static_cast<float>(pt.x), static_cast<float>(pt.y) };
	}

	constexpr int WrapCoord(int coord, int range_start, int range_size)
	{
		while (coord >= range_start + range_size) coord -= range_size;
		while (coord < range_start) coord += range_size;
		return coord;
	}

	constexpr Point WrapPoint(Point pos, Rect area)
	{
		return { WrapCoord(pos.x, area.x, area.w), WrapCoord(pos.y, area.y, area.h) };
	}

	namespace std {
		template <> struct hash<Point> {
			size_t operator()(Point pos) const {
				auto val = std::bit_cast<std::uint64_t>(pos);
				return std::hash<std::uint64_t>{}(val);
			}
		};
	}
}