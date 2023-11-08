export module utils:Array2D;

import :Math;

import std;

export
{
	template<class Elem>
	class Array2D
	{
		using ilist = std::initializer_list<Elem>;
	public:
		// Constructors

		Array2D() = default;

		explicit Array2D(const Rect& rect)
			: area(rect)
		{
			if (rect.w > 0 && rect.h > 0) {
				data.reset(new Elem[rect.w * rect.h]);
			}
		}

		Array2D(const Rect& rect, const auto& fill_value)
			: Array2D(rect)
		{
			std::fill(begin(), end(), fill_value);
		}

		Array2D(int w, int h)
			: Array2D(Rect{ 0, 0, w, h })
		{}

		Array2D(int w, int h, const auto& fill_value)
			: Array2D(Rect{ 0, 0, w, h }, fill_value)
		{}

		// Move and copy

		Array2D(const Array2D& rhs)
			: Array2D(rhs.area)
		{
			std::copy(rhs.begin(), rhs.end(), begin());
		}

		Array2D(Array2D&& rhs) noexcept
			: area(rhs.area)
			, data(std::move(rhs.data))
		{}

		Array2D& operator=(const Array2D& rhs)
		{
			area = rhs.area;
			data.reset(new Elem[area.w * area.h]);
			std::copy(rhs.begin(), rhs.end(), begin());
			return *this;
		}

		Array2D& operator=(Array2D&& rhs) noexcept
		{
			area = rhs.area;
			data = std::move(rhs.data);
			return *this;
		}

		// Comparison

		bool operator==(const Array2D& rhs) const
		{
			return area == rhs.area && std::equal(begin(), end(), rhs.begin(), rhs.end());
		}

		// Elements access

		const Elem& operator[](Point pt) const { return data[PosToIndex(pt)]; }
		Elem& operator[](Point pt) { return data[PosToIndex(pt)]; }

		auto begin() { return data.get(); }
		auto end() { return data.get() + area.w * area.h; }
		auto begin() const { return data.get(); }
		auto end() const { return data.get() + area.w * area.h; }

		// Area & Size

		const Rect& Area() const { return area; }
		Point Size() const { return { area.w, area.h }; }

		void SetOrigin(Point pt) {
			area.x = pt.x;
			area.y = pt.y;
		}

	private:
		Rect area;
		std::unique_ptr<Elem[]> data;

		size_t PosToIndex(Point pt) const {
			return (pt.y - area.y) * area.w + (pt.x - area.x);
		}
	};

	template<class Pred>
	auto Array2DFromString(std::string_view str, Pred&& pred, char separator = '\n')
	{
		const auto lines = str
			| std::views::split(separator)
			| std::ranges::to<std::vector>();

		const auto h = lines.size();
		const auto w = lines.empty() ? 0 : lines[0].size();

		Array2D<std::invoke_result_t<Pred, char>> out(static_cast<int>(w), static_cast<int>(h));

		auto data = lines
			| std::views::reverse
			| std::views::join
			| std::views::transform(std::forward<Pred>(pred));

		std::ranges::copy(data, out.begin());

		return out;
	}

	template<class T, class Pred>
	auto TransformArray2D(const Array2D<T>& arr, Pred pred)
	{
		using RetType = std::invoke_result_t<Pred, T>;
		Array2D<RetType> out(arr.Area());
		std::transform(arr.begin(), arr.end(), out.begin(), pred);
		return out;
	}
}