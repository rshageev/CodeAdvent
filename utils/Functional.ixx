export module utils:Functional;

import std;

export
{
	namespace stdr = std::ranges;
	namespace stdv = std::ranges::views;

	/* Converting strings to numbers */

	template<std::integral T>
	struct from_chars_to
	{
		constexpr T operator() (char ch) const {
			return static_cast<T>(ch - '0');
		}

		constexpr T operator() (auto&& v) const {
			std::string_view str(v);
			T value = def;
			std::from_chars(str.data(), str.data() + str.size(), value);
			return value;
		}
		T def = 0;
	};	
	inline constexpr from_chars_to<int> to_int;
	inline constexpr from_chars_to<std::uint64_t> to_uint64;
	inline constexpr from_chars_to<std::int64_t> to_int64;


	/* Accessing keys / values/ tuple elements */
	template<size_t N>
	struct tuple_get_fn {
		constexpr auto operator() (auto&& t) { return std::get<N>(t); }
	};
	template<size_t N>
	inline constexpr tuple_get_fn<N> tuple_get;

	/* Comparison predicates */
	template<auto V>
	struct equal_fn {
		constexpr bool operator() (auto&& v) const { return v == V; }
	};
	template<auto V>
	inline constexpr equal_fn<V> equal;

	template<auto V>
	struct not_equal_fn {
		constexpr bool operator() (auto&& v) const { return v != V; }
	};
	template<auto V>
	inline constexpr not_equal_fn<V> not_equal;

	/* Convert binary predicated into unary working on pair */
	template<class P>
	struct apply_to_pair {
		P pred;
		auto operator() (auto&& p) {
			auto& [p1, p2] = p;
			return std::invoke(pred, p1, p2);
		}
	};

	template<class P>
	struct apply_to_pair_rev {
		P pred;
		auto operator() (auto&& p) {
			auto& [p1, p2] = p;
			return std::invoke(pred, p2, p1);
		}
	};

	/* Construct an object from fiven arguments */
	template<class T>
	struct make_fn {
		template<class ...Args>
		constexpr T operator()(Args&&... args) const {
			return T{ std::forward<Args>(args)... };
		}
	};
	template<class T>
	inline constexpr make_fn<T> make;


	/* Misc utility functions */

	constexpr bool is_digit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	constexpr bool contains(const auto& range, const auto& v) {
		return stdr::find(range, v) != stdr::end(range);
	}

	
	struct sum_fn
	{
		template<typename Rng>
			requires std::integral<std::ranges::range_value_t<Rng>>
		constexpr auto operator()(const Rng& range) const {
			using ValueT = std::ranges::range_value_t<Rng>;
			return std::accumulate(std::begin(range), std::end(range), static_cast<ValueT>(0));
		}
	};
	inline constexpr sum_fn Sum;
}