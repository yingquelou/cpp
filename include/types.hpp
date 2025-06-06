#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP 1
#include <type_traits>
#include "compiler_detect.h"
namespace types
{
	// 类型集合,作为结果类型被使用
	template <typename...>
	struct types
	{
	};
	namespace __detail
	{
		template <typename, typename _B1, typename... _Bn>
		struct __disjunction_impl
		{
			using type = _B1;
		};

		template <typename _B1, typename _B2, typename... _Bn>
		struct __disjunction_impl<typename std::enable_if<!bool(_B1::value)>::type, _B1, _B2, _Bn...>
		{
			using type = typename __disjunction_impl<void, _B2, _Bn...>::type;
		};
		template <typename, typename _B1, typename... _Bn>

		struct __conjunction_impl
		{
			using type = _B1;
		};

		template <typename _B1, typename _B2, typename... _Bn>
		struct __conjunction_impl<typename std::enable_if<bool(_B1::value)>::type, _B1, _B2, _Bn...>
		{
			using type = typename __conjunction_impl<void, _B2, _Bn...>::type;
		};

		template <template <typename> class, typename...>
		struct __filter_impl;
		// predicate
		template <template <typename> class Predicate,
				  typename T,
				  typename... Ts1,
				  typename... Ts2>
		struct __filter_impl<Predicate, types<Ts1...>, types<T, Ts2...>>
		{
			using type = typename std::conditional<bool(Predicate<T>::value),
												   typename __filter_impl<Predicate, types<Ts1..., T>, types<Ts2...>>::type,
												   typename __filter_impl<Predicate, types<Ts1...>, types<Ts2...>>::type>::type;
		};
		template <template <typename> class Predicate,
				  typename... Ts>
		struct __filter_impl<Predicate, types<Ts...>, types<>>
		{
			using type = types<Ts...>;
		};
	}

	template <typename... _Bn>
	struct disjunction : __detail::__disjunction_impl<void, _Bn...>::type
	{
	};

	template <>
	struct disjunction<> : std::false_type
	{
	};

	template <typename... _Bn>
	struct conjunction : __detail::__conjunction_impl<void, _Bn...>::type
	{
	};

	template <>
	struct conjunction<> : std::false_type
	{
	};

	template <template <typename> class, typename...>
	struct filter;
	template <
		template <typename> class Predicate,
		template <typename...> class Container,
		typename... Ts>
	struct filter<Predicate, Container<Ts...>>
	{
		using type = typename __detail::__filter_impl<Predicate, types<>, Container<Ts...>>::type;
	};

	template <
		template <typename> class Predicate,
		typename Container>
	using filter_t = typename filter<Predicate, Container>::type;
#if CPP_STANDARD >= STD_CXX14
	template <typename... _Bn>
	inline constexpr bool disjunction_v = disjunction<_Bn...>::value;
	template <typename... _Bn>
	inline constexpr bool conjunction_v = conjunction<_Bn...>::value;
#endif
}
namespace types
{
	namespace __detail
	{
		template <typename...>
		struct __types_n_helper;
		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			template <typename...> class Tv3,
			typename... Ts1,
			typename... Ts2,
			typename... Ts3,
			typename T>
		struct __types_n_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<T, Ts3...>>
		{
			using type = typename std::conditional<bool(disjunction<std::is_same<T, Ts2>...>::value), typename __types_n_helper<Tv1<Ts1..., T>, Tv2<Ts2...>, Tv3<Ts3...>>::type, typename __types_n_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<Ts3...>>::type>::type;
		};
		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			template <typename...> class Tv3,
			typename... Ts1,
			typename... Ts2>
		struct __types_n_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<>>
		{
			using type = Tv1<Ts1...>;
		};

		template <typename...>
		struct __types_x_helper;
		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			template <typename...> class Tv3,
			typename... Ts1,
			typename... Ts2,
			typename... Ts3,
			typename T>
		struct __types_x_helper<Tv1<Ts1...>, Tv2<T, Ts2...>, Tv3<Ts3...>>
		{
			using type = typename std::conditional<bool(disjunction<std::is_same<T, Ts3>...>::value), typename __types_x_helper<Tv1<Ts1...>, Tv2<Ts2...>, Tv3<Ts3...>>::type, typename __types_x_helper<Tv1<Ts1..., T>, Tv2<Ts2...>, Tv3<Ts3...>>::type>::type;
		};
		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			template <typename...> class Tv3,
			typename... Ts1,
			typename... Ts3>
		struct __types_x_helper<Tv1<Ts1...>, Tv2<>, Tv3<Ts3...>>
		{
			using type = Tv1<Ts1...>;
		};

		template <typename...>
		struct __types_u_helper;

		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			typename... Ts1,
			typename... Ts2,
			typename T>
		struct __types_u_helper<Tv1<Ts1...>, Tv2<T, Ts2...>>
		{
			using type = typename std::conditional<bool(disjunction<std::is_same<T, Ts1>...>::value), typename __types_u_helper<Tv1<Ts1...>, Tv2<Ts2...>>::type, typename __types_u_helper<Tv1<Ts1..., T>, Tv2<Ts2...>>::type>::type;
		};
		template <
			template <typename...> class Tv1,
			template <typename...> class Tv2,
			typename... Ts>
		struct __types_u_helper<Tv1<Ts...>, Tv2<>>
		{
			using type = Tv1<Ts...>;
		};

	} // namespace __detail

	// 交集
	template <typename...>
	struct types_n;
	template <
		template <typename...> class Tv1,
		template <typename...> class Tv3,
		typename... Ts1,
		typename... Ts3,
		typename T>
	struct types_n<Tv1<Ts1...>, T, Tv3<Ts3...>>
	{
		using type = typename __detail::__types_n_helper<types<Ts1...>, T, types<Ts3...>>::type;
	};
	// 交集
	template <typename U1, typename U2>
	using types_n_t = typename types_n<types<>, U1, U2>::type;

	// 差集
	template <typename...>
	struct types_x;
	template <
		template <typename...> class Tv1,
		template <typename...> class Tv3,
		typename... Ts1,
		typename... Ts3,
		typename T>
	struct types_x<Tv1<Ts1...>, T, Tv3<Ts3...>>
	{
		using type = typename __detail::__types_x_helper<types<Ts1...>, T, types<Ts3...>>::type;
	};
	// 差集
	template <typename U1, typename U2>
	using types_x_t = typename types_x<types<>, U1, U2>::type;

	// 并集
	template <typename...>
	struct types_u;
	template <
		template <typename...> class Tv1,
		template <typename...> class Tv2,
		typename... Ts1,
		typename... Ts2>
	struct types_u<Tv1<Ts1...>, Tv2<Ts2...>>
	{
		using type = typename __detail::__types_u_helper<types<>, types<Ts1..., Ts2...>>::type;
	};
	// 并集
	template <typename _tuple1, typename _tuple2>
	using types_u_t = typename types_u<_tuple1, _tuple2>::type;
} // namespace types
#endif
