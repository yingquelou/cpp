#pragma once
#include <tuple>
namespace types
{
	namespace __detail
	{
		template <typename...>
		struct __tuple_n_helper;
		template <
			template <typename...> typename Tv1,
			template <typename...> typename Tv2,
			template <typename...> typename Tv3,
			typename... Ts1,
			typename... Ts2,
			typename... Ts3,
			typename T>
		struct __tuple_n_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<T, Ts3...>>
		{
			using type = std::conditional_t<std::disjunction_v<std::is_same<T, Ts2>...>, typename __tuple_n_helper<Tv1<Ts1..., T>, Tv2<Ts2...>, Tv3<Ts3...>>::type, typename __tuple_n_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<Ts3...>>::type>;
		};
		template <
			template <typename...> typename Tv1,
			template <typename...> typename Tv2,
			template <typename...> typename Tv3,
			typename... Ts1,
			typename... Ts2>
		struct __tuple_n_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<>>
		{
			using type = Tv1<Ts1...>;
		};

		template <typename...>
		struct __tuple_x_helper;
		template <
			template <typename...> typename Tv1,
			template <typename...> typename Tv2,
			template <typename...> typename Tv3,
			typename... Ts1,
			typename... Ts2,
			typename... Ts3,
			typename T>
		struct __tuple_x_helper<Tv1<Ts1...>, Tv2<T, Ts2...>, Tv3<Ts3...>>
		{
			using type = std::conditional_t<std::disjunction_v<std::is_same<T, Ts3>...>, typename __tuple_x_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<Ts3...>>::type, typename __tuple_x_helper<Tv1<Ts1..., T>, Tv2<Ts2...>, Tv3<Ts3...>>::type>;
		};
		template <
			template <typename...> typename Tv1,
			template <typename...> typename Tv2,
			template <typename...> typename Tv3,
			typename... Ts1,
			typename... Ts3>
		struct __tuple_x_helper<Tv1<Ts1...>, Tv2<>, Tv3<Ts3...>>
		{
			using type = Tv1<Ts1...>;
		};

		template <typename...>
		struct _tuple_u_helper;

		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			typename... Ts1,
			typename... Ts2,
			typename T>
		struct _tuple_u_helper<Tv1<Ts1...>, Tv2<T, Ts2...>>
		{
			using type = std::conditional_t<std::disjunction_v<std::is_same<T, Ts1>...>, typename _tuple_u_helper<Tv1<Ts1...>, Tv2<Ts2...>>::type, typename _tuple_u_helper<Tv1<Ts1..., T>, Tv2<Ts2...>>::type>;
		};
		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			typename... Ts>
		struct _tuple_u_helper<Tv1<Ts...>, Tv2<>>
		{
			using type = Tv1<Ts...>;
		};

	} // namespace __detail
	// 交集
	template <typename...>
	struct tuple_n;
	template <
		template <typename...> typename Tv1,
		template <typename...> typename Tv3,
		typename... Ts1,
		typename... Ts3,
		typename T>
	struct tuple_n<Tv1<Ts1...>, T, Tv3<Ts3...>>
	{
		using type = typename __detail::__tuple_n_helper<std::tuple<Ts1...>, T, std::tuple<Ts3...>>::type;
	};
	template <typename U1, typename U2>
	using tuple_n_t = typename tuple_n<std::tuple<>, U1, U2>::type;

	// 差集
	template <typename...>
	struct tuple_x;
	template <
		template <typename...> typename Tv1,
		template <typename...> typename Tv3,
		typename... Ts1,
		typename... Ts3,
		typename T>
	struct tuple_x<Tv1<Ts1...>, T, Tv3<Ts3...>>
	{
		using type = typename __detail::__tuple_x_helper<std::tuple<Ts1...>, T, std::tuple<Ts3...>>::type;
	};
	template <typename U1, typename U2>
	using tuple_x_t = typename tuple_x<std::tuple<>, U1, U2>::type;

	// 并集
	template <typename...>
	struct tuple_u;
	template <
		template <typename...> class Tv1,
		template <typename...> class Tv2,
		typename... Ts1,
		typename... Ts2>
	struct tuple_u<Tv1<Ts1...>, Tv2<Ts2...>>
	{
		using type = typename __detail::_tuple_u_helper<std::tuple<>, std::tuple<Ts1..., Ts2...>>::type;
	};
	template <typename _tuple1, typename _tuple2>
	using tuple_u_t = typename tuple_u<_tuple1, _tuple2>::type;
} // namespace types
