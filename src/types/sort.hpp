#ifndef HYBRID_TYPE_SORT_HPP
#define HYBRID_TYPE_SORT_HPP 1
#include "types.hpp"
namespace types
{
	namespace sort
	{
		// 假定Comparator为<或<=语义,衍生出<,>,=,<=,>=
		template <template <typename, typename> class Comparator>
		class ComparatorAdapter
		{
			template <typename L, typename R>
			using _less = Comparator<L, R>;
			template <typename L, typename R>
			using _greater = Comparator<R, L>;

		public:
			template <typename L, typename R>
			using equal = std::integral_constant<bool, !(bool(_less<L, R>::value) ^ bool(_greater<L, R>::value))>;
			template <typename L, typename R>
			using less = std::integral_constant<bool, _less<L, R>::value && !equal<L, R>::value>;
			template <typename L, typename R>
			using less_equal = std::integral_constant<bool,_less<L, R>::value || equal<L, R>::value>;
			template <typename L, typename R>
			using greater = std::integral_constant<bool, _greater<L, R>::value && !equal<L, R>::value>;
			template <typename L, typename R>
			using greater_equal = std::integral_constant<bool, _greater<L, R>::value || equal<L, R>::value>;
		};
		// 以不同方式实现了排序

		namespace __impl_insert
		{
			// Predicate的实例是一个BinaryTypeTrait
			template <template <typename, typename> class Predicate, typename Tuple>
			struct sort_insert;

			template <template <typename, typename> class Predicate>
			struct sort_insert<Predicate, std::tuple<>>
			{
				using type = std::tuple<>;
			};

			template <template <typename, typename> class Predicate, typename T>
			struct sort_insert<Predicate, std::tuple<T>>
			{
				using type = std::tuple<T>;
			};

			template <template <typename, typename> class Predicate, typename T, typename... Ts>
			struct sort_insert<Predicate, std::tuple<T, Ts...>>
			{
			private:
				template <typename L, typename R>
				using cmp = typename ComparatorAdapter<Predicate>::template less_equal<L, R>;
				// 插入单个元素到已排序的元组中
				template <typename Elem, typename SortedTuple>
				struct inserter;

				template <typename Elem>
				struct inserter<Elem, std::tuple<>>
				{
					using type = std::tuple<Elem>;
				};

				template <typename Elem, typename First, typename... Rest>
				struct inserter<Elem, std::tuple<First, Rest...>>
				{
					using type = typename std::conditional<
						cmp<Elem, First>::value,
						std::tuple<Elem, First, Rest...>,
						decltype(std::tuple_cat(std::tuple<First>{}, typename inserter<Elem, std::tuple<Rest...>>::type{}))

						>::type;
				};

				using rest_sorted = typename sort_insert<Predicate, std::tuple<Ts...>>::type;

			public:
				using type = typename inserter<T, rest_sorted>::type;
			};
		}

		using __impl_insert::sort_insert;

		namespace __impl_bubble
		{
			template <template <typename, typename> class Predicate, typename Tuple>
			struct sort_bubble
			{
			private:
				template <typename L, typename R>
				using cmp = typename ComparatorAdapter<Predicate>::template less_equal<L, R>;
				// 单次冒泡遍历
				template <std::size_t, typename CurrentTuple, typename = void>
				struct bubble_pass
				{
					typedef CurrentTuple type;
				};

				// 递归冒泡实现
				template <std::size_t Pass, typename First, typename Second, typename... Rest>
				struct bubble_pass<Pass, std::tuple<First, Second, Rest...>, typename std::enable_if<(Pass > 1)>::type>
				{
				private:
					typedef typename std::conditional<cmp<First, Second>::value,
													  std::pair<First, Second>,
													  std::pair<Second, First>>::type after_compare;
					using rest_sorted = typename bubble_pass<Pass - 1, std::tuple<typename after_compare::second_type, Rest...>>::type;

				public:
					using type = decltype(std::tuple_cat(std::tuple<typename after_compare::first_type>{}, rest_sorted{}));
				};

				// 执行固定次数的冒泡排序
				template <std::size_t RemainingPasses, typename CurrentTuple>
				struct bubble_sort_impl
				{
				private:
					typedef typename bubble_pass<RemainingPasses, CurrentTuple>::type after_pass;

				public:
					typedef typename bubble_sort_impl<RemainingPasses - 1, after_pass>::type type;
				};

				template <typename CurrentTuple>
				struct bubble_sort_impl<0, CurrentTuple>
				{
					typedef CurrentTuple type;
				};

			public:
				typedef typename bubble_sort_impl<std::tuple_size<Tuple>::value, Tuple>::type type;
			};
		} // namespace __impl_bubble

		using __impl_bubble::sort_bubble;

		namespace __impl_counting
		{
			template <template <typename, typename> class Predicate, typename Tuple>
			struct sort_count
			{
			private:
				// 元素:元素计数
				template <typename T, std::size_t Ct>
				struct entry : private std::integral_constant<std::size_t, Ct>
				{
					// 元素
					using type = T;
					// 元素计数
					using std::integral_constant<std::size_t, Ct>::value;
				};
				template <typename T, std::size_t Index, typename CurTuple>
				class count_smaller
				{
					template <typename _T, std::size_t _Index, typename>
					using is_true = std::integral_constant<bool, Index != _Index && Predicate<_T, T>::value>;
					using trues = typename filter_include<is_true, CurTuple>::type;

				public:
					using type = entry<T, std::tuple_size<trues>::value>;
				};

				// 递归构建最终元组
				template <std::size_t Rank, typename CurEntries>
				class build_sorted_tuple
				{
					template <typename T, std::size_t, typename>
					using is_equal = std::integral_constant<bool, (Rank == T::value)>;
					// 分离具有指定计数的元素
					using splited = typename filter<is_equal, CurEntries>::type;
					template <typename T, std::size_t, typename>
					using extract_type = as_type<typename T::type>;
					using first_type = typename map<extract_type, typename splited::first_type>::type;

				public:
					using type = typename merge<first_type,
												typename build_sorted_tuple<Rank + 1, typename splited::second_type>::type>::type;
				};
				template <std::size_t Rank>
				class build_sorted_tuple<Rank, std::tuple<>>
				{
				public:
					using type = std::tuple<>;
				};

			public:
				using type = typename build_sorted_tuple<0, typename map<count_smaller, Tuple>::type>::type;
			};
		}

		using __impl_counting::sort_count;

		namespace __impl_iterative
		{
			// 主排序模板
			template <template <typename, typename> class Predicate, typename Tuple>
			struct sort_group
			{
			private:
				template <typename CurTuple>
				class unique	
				{
				public:
					using type = CurTuple;
				};
				template <typename T, typename... Ts>
				class unique<std::tuple<T, Ts...>>
				{
					template <typename _T,std::size_t,typename>
					using equal= typename ComparatorAdapter<Predicate>::template equal<_T, T>;
					using splited = typename filter<equal,std::tuple<Ts...>>::type;
					using equals_tuple = typename merge<std::tuple<T>,typename splited::first_type>::type;

				public:
					using type = typename merge<
									std::tuple<equals_tuple>,
									typename unique<typename splited::second_type>::type
								>::type;
				};

				template <typename CurTuple, typename OtherTuple>
				class find_min_type;
				template <typename First, typename Second, typename... Ts1, typename... Ts2>
				class find_min_type<std::tuple<First, Second, Ts1...>, std::tuple<Ts2...>>
				{
					using cmp = Predicate<typename std::tuple_element<0, First>::type, 
											typename std::tuple_element<0, Second>::type>;
					using cmp_result = typename std::conditional<cmp::value,
																 std::pair<First, Second>,
																 std::pair<Second, First>>::type;
				public:
					using type = typename find_min_type<std::tuple<typename cmp_result::first_type, Ts1...>,
														std::tuple<Ts2..., typename cmp_result::second_type>>::type;
				};
				template <typename T, typename OtherTuple>
				class find_min_type<std::tuple<T>, OtherTuple>
				{
				public:
					using type = std::pair<T, OtherTuple>;
				};

				// 构建排序后的元组类型
				template <typename CurTuple>
				class build
				{
					using splited = typename find_min_type<CurTuple, std::tuple<>>::type;

				public:
					using type = typename merge<typename splited::first_type,
												typename build<typename splited::second_type>::type>::type;
				};
				template <>
				class build<std::tuple<>>
				{
				public:
					using type = std::tuple<>;
				};

			public:
				using type = typename build<typename unique<Tuple>::type>::type;
			};
		}
		using __impl_iterative::sort_group;

		namespace __impl_hybrid
		{
			// 选择最适合的排序算法基于规模
			template <template <typename, typename> class Predicate, typename Tuple, bool Large = (std::tuple_size<Tuple>::value > 8)>
			struct hybrid_type_sort_impl;

			// 小规模使用选择排序
			template <template <typename, typename> class Predicate, typename Tuple>
			struct hybrid_type_sort_impl<Predicate, Tuple, false>
			{
				typedef typename sort_group<Predicate, Tuple>::type type;
			};

			// 大规模使用计数排序
			template <template <typename, typename> class Predicate, typename Tuple>
			struct hybrid_type_sort_impl<Predicate, Tuple, true>
			{
				typedef typename sort_count<Predicate, Tuple>::type type;
			};

			// 主入口
			template <template <typename, typename> class Predicate, typename Tuple>
			struct sort_hybrid
			{
				typedef typename hybrid_type_sort_impl<Predicate, Tuple>::type type;
			};
		}

		using __impl_hybrid::sort_hybrid;

		namespace __impl_is_sorted
		{
			/*
				`type`: 判断元组是否有序
				- `Predicate`: 模板参数,其实例满足约定`type`
				- `Tuple`: 类型参数,原集合
			 */
			template <template <typename, typename> class /* value */ Predicate,
					  typename Tuple>
			class is_sorted
			{
				template <typename L, typename R>
				using cmp = typename ComparatorAdapter<Predicate>::template less_equal<L, R>;
				template <typename CurTuple>
				struct helper;
				template <typename First, typename Second>
				struct helper<std::tuple<First, Second>>
					: cmp<First, Second>
				{
					// static_assert(0,"?");
				};
				template <typename First, typename Second, typename... Ts>
				struct helper<std::tuple<First, Second, Ts...>>
					: conjunction<cmp<First, Second>, helper<std::tuple<Second, Ts...>>>
				{
					// static_assert(0,"?");
				};

			public:
				static const std::size_t value = helper<Tuple>::value;
			};
		}

		using __impl_is_sorted::is_sorted;

		namespace __impl_heap
		{
			// 堆排序
			template <template <typename, typename> class Predicate, typename Tuple>
			class sort_heap
			{
				template <typename L, typename R>
				using cmp = typename ComparatorAdapter<Predicate>::template less<L, R>;
				template <typename CurTuple>
				class helper
				{
				public:
					using type = CurTuple;
				};
				template <typename T, typename... Ts>
				class helper<std::tuple<T, Ts...>>
				{
					template <typename _T, std::size_t, typename>
					using is_small = std::integral_constant<bool, cmp<_T, T>::value>;
					using splited = typename filter<is_small, std::tuple<Ts...>>::type;

				public:
					using type = typename merge<
						typename helper<typename splited::first_type>::type,
						std::tuple<T>,
						typename helper<typename splited::second_type>::type>::type;
				};

			public:
				using type = typename helper<Tuple>::type;
			};
		}

		using __impl_heap::sort_heap;
	} // namespace sort
} // namespace types
#endif