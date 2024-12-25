export module utils:Array2D;

import :Math;
import :File;

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

		std::span<Elem> GetRow(int y) {
			return { data.get() + (y - area.y) * area.w, static_cast<size_t>(area.w) };
		}
		std::span<const Elem> GetRow(int y) const {
			return { data.get() + (y - area.y) * area.w, static_cast<size_t>(area.w) };
		}

		auto ColumnIndices() const { return std::views::iota(area.x, area.x + area.w); }
		auto RowIndices() const { return std::views::iota(area.y, area.y + area.h); }

		auto begin() { return data.get(); }
		auto end() { return data.get() + area.w * area.h; }
		auto begin() const { return data.get(); }
		auto end() const { return data.get() + area.w * area.h; }


		// Area & Size

		const Rect& Area() const { return area; }
		Point Size() const { return { area.w, area.h }; }
		int Width() const { return area.w; }
		int Height() const { return area.h; }

		void SetOrigin(Point pt) {
			area.x = pt.x;
			area.y = pt.y;
		}

		bool Contains(Point pt) const {
			return ::Contains(area, pt);
		}

	private:
		Rect area;
		std::unique_ptr<Elem[]> data;

		size_t PosToIndex(Point pt) const {
			return (pt.y - area.y) * area.w + (pt.x - area.x);
		}
	};

	template<class R, class Pred = std::identity>
	auto Array2DFromLines(R&& lines_rng, Pred&& pred = {})
	{
		const auto lines = lines_rng | std::ranges::to<std::vector>();

		const auto h = lines.size();
		const auto w = lines.empty() ? 0 : lines[0].size();

		using OutType = std::remove_reference_t<std::invoke_result_t<Pred, char>>;
		Array2D<OutType> out(static_cast<int>(w), static_cast<int>(h));

		auto data = lines
			| std::views::reverse
			| std::views::join
			| std::views::transform(std::forward<Pred>(pred));

		std::ranges::copy(data, out.begin());

		return out;
	}

	template<class Pred = std::identity>
	auto Array2DFromString(std::string_view str, Pred&& pred = {}, char separator = '\n')
	{
		const auto lines = str
			| std::views::split(separator)
			| std::ranges::to<std::vector>();

		const auto h = lines.size();
		const auto w = lines.empty() ? 0 : lines[0].size();

		using OutType = std::remove_reference_t<std::invoke_result_t<Pred, char>>;
		Array2D<OutType> out(static_cast<int>(w), static_cast<int>(h));

		auto data = lines
			| std::views::reverse
			| std::views::join
			| std::views::transform(std::forward<Pred>(pred));

		std::ranges::copy(data, out.begin());

		return out;
	}

	template<class T>
	void FlipArrayVertical(Array2D<T>& arr)
	{
		const Rect rect = arr.Area();
		for (int y = 0; y < rect.h / 2; ++y)
		{
			for (int x = 0; x < rect.w; ++x)
			{
				Point p1{ rect.x + x, rect.y + y };
				Point p2{ rect.x + x, rect.y + rect.h - y - 1 };
				std::swap(arr[p1], arr[p2]);
			}
		}
	}

	template<class T, class Pred>
	auto TransformArray2D(const Array2D<T>& arr, Pred pred)
	{
		using RetType = std::invoke_result_t<Pred, T>;
		Array2D<RetType> out(arr.Area());
		std::transform(arr.begin(), arr.end(), out.begin(), pred);
		return out;
	}

	template<class T>
	void CopyArray2D(const Array2D<T>& src, Array2D<T>& dst)
	{
		const Rect src_rect = src.Area();
		for (int y = src_rect.y; y < src_rect.y + src_rect.h; ++y) {
			for (int x = src_rect.x; x < src_rect.x + src_rect.w; ++x) {
				Point pos{ x, y };
				dst[pos] = src[pos];
			}
		}
	}

	template<class T, class Pred = std::identity>
	void PrintArray2D(const Array2D<T>& arr, Pred&& to_char = {}, std::ostream& out = std::cout)
	{
		const Rect rect = arr.Area();
		std::string out_str;
		out_str.reserve(rect.w * rect.h);
		for (int y = rect.y + rect.h - 1; y >= rect.y; --y) {
			for (int x = rect.x; x < rect.x + rect.w; ++x) {
				Point pos{ x, y };
				out_str.push_back(std::invoke(to_char, arr[pos]));
			}
			out_str.push_back('\n');
		}
		out << out_str;
	}

	template<class Pred>
	void PrintData2D(Rect rect, Pred&& to_char, std::ostream& out = std::cout)
	{
		std::string out_str;
		out_str.reserve(rect.w * rect.h);
		for (int y = rect.y + rect.h - 1; y >= rect.y; --y) {
			for (int x = rect.x; x < rect.x + rect.w; ++x) {
				Point pos{ x, y };
				out_str.push_back(std::invoke(to_char, pos));
			}
			out_str.push_back('\n');
		}
		out << out_str;
	}

	template<class Elem, class T>
	Point FindInArray2D(const Array2D<Elem>& arr, const T& value)
	{
		const Rect rect = arr.Area();
		Point pos;
		for (pos.y = rect.y; pos.y < rect.y + rect.h; ++pos.y) {
			for (pos.x = rect.x; pos.x < rect.x + rect.w; ++pos.x) {
				if (arr[pos] == value) return pos;
			}
		}
		return pos;
	}

	template<class Pred = std::identity>
	auto ReadArray2D(const std::filesystem::path& filename, Pred&& pred = {}) {
		return Array2DFromString(ReadText(filename), std::forward<Pred>(pred));
	}
}