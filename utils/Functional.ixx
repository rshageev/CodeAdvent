export module utils:Functional;

import std;

export
{
	namespace stdr = std::ranges;
	namespace stdv = std::ranges::views;

	struct pred_not_empty {
		constexpr bool operator() (auto&& v) { return !v.empty(); }
	};

	struct pred_is_empty {
		constexpr bool operator() (auto&& v) { return v.empty(); }
	};
	inline constexpr pred_not_empty not_empty;
	inline constexpr pred_is_empty is_empty;

	template<std::integral T>
	struct from_chars_to {
		constexpr int operator() (auto&& v) const {
			std::string_view str(v);
			T value = def;
			std::from_chars(str.data(), str.data() + str.size(), value);
			return value;
		}
		T def = 0;
	};	
	inline constexpr from_chars_to<int> to_int;

	template<size_t N>
	struct tuple_get_fn {
		constexpr auto operator() (auto&& t) { return std::get<N>(t); }
	};
	template<size_t N>
	inline constexpr tuple_get_fn<N> tuple_get;

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

	template<class T>
	struct make_fn {
		template<class ...Args>
		constexpr T operator()(Args&&... args) const {
			return T{ std::forward<Args>(args)... };
		}
	};
	template<class T>
	inline constexpr make_fn<T> make;


	constexpr bool is_digit(char ch) {
		return ch >= '0' && ch <= '9';
	}

	constexpr bool contains(const auto& range, const auto& v) {
		return stdr::find(range, v) != stdr::end(range);
	}
}