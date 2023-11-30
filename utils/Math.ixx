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

		friend TPoint operator+ (TPoint pt1, TPoint pt2) { return { pt1.x + pt2.x, pt1.y + pt2.y }; }
		friend TPoint operator- (TPoint pt1, TPoint pt2) { return { pt1.x - pt2.x, pt1.y - pt2.y }; }

		friend TPoint operator* (TPoint pt, T v) { return { pt.x * v, pt.y * v }; }
		friend TPoint operator* (T v, TPoint pt) { return { pt.x * v, pt.y * v }; }

		friend TPoint operator/ (TPoint pt, T v) { return { pt.x / v, pt.y / v }; }
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

	using Point = TPoint<int>;
	using Pointf = TPoint<float>;
	using Rect = TRect<int>;
	using Rectf = TRect<float>;

	Pointf ToFloat(Point pt)
	{
		return { static_cast<float>(pt.x), static_cast<float>(pt.y) };
	}

	constexpr Point WrapPoint(Point pos, Rect area)
	{
		while (pos.x >= area.x + area.w) pos.x -= area.w;
		while (pos.x < area.x) pos.x += area.w;
		while (pos.y >= area.y + area.h) pos.y -= area.h;
		while (pos.y < area.y) pos.y += area.h;
		return pos;
	}
}