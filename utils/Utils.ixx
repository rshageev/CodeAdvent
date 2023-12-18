export module utils;

export import std;

export import :Math;
export import :Array2D;
export import :RectViews;
export import :Timer;
export import :Functional;
export import :Directions;

export
{
	namespace stdr = std::ranges;
	namespace stdv = std::ranges::views;

	/* File IO */
	std::string ReadText(const std::filesystem::path& filename);
	std::vector<std::string> ReadLines(const std::filesystem::path& filename);

	template<class Pred>
	auto ReadArray2D(const std::filesystem::path& filename, Pred&& pred) {
		return Array2DFromString(ReadText(filename), std::forward<Pred>(pred));
	}
	Array2D<char> ReadArray2D(const std::filesystem::path& filename) {
		return Array2DFromString(ReadText(filename), [](char ch) { return ch; });
	}

	/* Parsing */
	template<std::integral T, int base = 10>
	T Read(std::string_view& str, T def = 0) {
		T value = def;
		const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, base);
		str.remove_prefix(ptr - str.data());
		return value;
	}
	int ReadInt(std::string_view& str, int def = 0) { return Read<int>(str, def); }
	std::string_view ReadWord(std::string_view& str, const char* delims = " ,.;");
	bool Skip(std::string_view& str, std::string_view prefix);
}