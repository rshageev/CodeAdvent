export module utils;

export import std;

export import :File;
export import :Parsing;
export import :Math;
export import :Array2D;
export import :RectViews;
export import :Timer;
export import :Functional;
export import :Directions;
export import :Types;
export import :Algo;
export import :Regex;

export
{
	namespace stdr = std::ranges;
	namespace stdv = std::ranges::views;

	/* Hashing */

	constexpr size_t HashCombine(size_t hash1, size_t hash2)
	{
		return hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
	}

	struct PairHash {
		template<class T1, class T2>
		size_t operator() (const std::pair<T1, T2>& p) const {
			size_t h1 = std::hash<T1>{}(p.first);
			size_t h2 = std::hash<T2>{}(p.second);
			return HashCombine(h1, h2);
		}
	};
}
