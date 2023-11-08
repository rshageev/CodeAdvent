export module utils:RectViews;

import :Math;
import :Array2D;

import std;


export struct rect_iterator
{
	using iterator_concept = std::forward_iterator_tag;
	using difference_type = int;
	using value_type = Point;

	Point current;
	int start_x;
	int end_x;
	int end_y;

	[[nodiscard]] constexpr Point operator*() const noexcept {
		return current;
	}

	constexpr rect_iterator& operator++() noexcept {
		increment();
		return *this;
	}

	constexpr auto operator++(int) noexcept {
		auto tmp = *this;
		increment();
		return tmp;
	}

	[[nodiscard]] constexpr bool operator==(std::default_sentinel_t) const noexcept {
		return current.y == end_y;
	}

	[[nodiscard]] friend constexpr bool operator==(const rect_iterator& lhs, const rect_iterator& rhs) noexcept {
		return lhs.current == rhs.current;
	}

public:
	constexpr void increment()
	{
		++current.x;
		if (current.x == end_x) {
			current.x = start_x;
			++current.y;
		}
	}
};

static_assert(std::input_iterator<rect_iterator>);
static_assert(std::forward_iterator<rect_iterator>);


export class rect_points_view : public std::ranges::view_interface<rect_points_view>
{
public:
	constexpr rect_points_view(const Rect& rect)
		: area(rect)
	{}

	constexpr bool empty() const {
		return area.w == 0 && area.h == 0;
	}

	constexpr auto begin() {
		return rect_iterator{
			.current = LeftBottom(area),
			.start_x = area.x,
			.end_x = area.x + area.w,
			.end_y = area.y + area.h
		};
	}

	constexpr auto end() {
		return std::default_sentinel;
	}

private:
	Rect area;
};


struct to_cell_coords_fn
{
	constexpr auto operator()(const Rect& rect) const {
		return rect_points_view{ rect };
	}

	template<typename T>
	constexpr auto operator()(const Array2D<T>& arr) const {
		return rect_points_view{ arr.Area() };
	}
};

export inline constexpr to_cell_coords_fn to_cell_coords;
	