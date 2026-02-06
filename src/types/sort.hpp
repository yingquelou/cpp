#include <tuple>
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
      using equal = std::integral_constant<bool, !(static_cast<bool>(_less<L, R>::value) ^ static_cast<bool>(_greater<L, R>::value))>;
      template <typename L, typename R>
      using less =
          std::integral_constant<bool, _less<L, R>::value && !equal<L, R>::value>;
      template <typename L, typename R>
      using less_equal =
          std::integral_constant<bool, _less<L, R>::value || equal<L, R>::value>;
      template <typename L, typename R>
      using greater = std::integral_constant<bool, _greater<L, R>::value &&
                                                       !equal<L, R>::value>;
      template <typename L, typename R>
      using greater_equal =
          std::integral_constant<bool, _greater<L, R>::value || equal<L, R>::value>;
    };

    namespace __
    {
      template <typename Tuple, template <typename, typename> class Comparator>
      class unique
      {
        using cmp = ComparatorAdapter<Comparator>;
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
          using equal = typename cmp::template equal<_T, T>;
          using splited = typename filter<equal, std::tuple<Ts...>>::type;
          using equal_tuples =
              typename concat<std::tuple<T>, typename splited::first_type>::type;

        public:
          using type = typename concat<
              std::tuple<equal_tuples>,
              typename helper<typename splited::second_type>::type>::type;
        };

      public:
        using type = typename helper<Tuple>::type;
      };
    }

    // 以不同方式实现了排序

    namespace __impl_insert
    {
      template <typename Tuple, template <typename, typename> class Comparator>
      class sort_insert
      {
        template <typename CurTuple>
        struct helper;
        template <>
        struct helper<std::tuple<>>
        {
          using type = std::tuple<>;
        };
        template <typename T>
        struct helper<std::tuple<T>>
        {
          using type = std::tuple<T>;
        };
        template <typename L, typename R>
        using cmp = typename ComparatorAdapter<Comparator>::template less_equal<L, R>;
        template <typename T, typename... Ts>
        struct helper<std::tuple<T, Ts...>>
        {
        private:
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
                cmp<Elem, First>::value, std::tuple<Elem, First, Rest...>,
                decltype(std::tuple_cat(
                    std::tuple<First>{},
                    typename inserter<Elem, std::tuple<Rest...>>::type{}))>::type;
          };
          using rest_sorted = typename helper<std::tuple<Ts...>>::type;

        public:
          using type = typename inserter<T, rest_sorted>::type;
        };

      public:
        using type = typename helper<Tuple>::type;
      };
    } // namespace __impl_insert
    using __impl_insert::sort_insert;
    namespace __impl_bubble
    {
      template <typename Tuple, template <typename, typename> class Comparator>
      struct sort_bubble
      {
      private:
        template <typename L, typename R>
        using cmp = typename ComparatorAdapter<Comparator>::template less_equal<L, R>;
        // 单次冒泡
        template <std::size_t, typename CurrentTuple, typename = void>
        struct bubble_pass
        {
          typedef CurrentTuple type;
        };
        template <std::size_t Pass, typename First, typename Second, typename... Rest>
        struct bubble_pass<Pass, std::tuple<First, Second, Rest...>,
                           typename std::enable_if<(Pass > 1)>::type>
        {
        private:
          using after_compare =
              typename std::conditional<cmp<First, Second>::value,
                                        std::pair<First, Second>,
                                        std::pair<Second, First>>::type;
          using rest_sorted = typename bubble_pass<
              Pass - 1,
              std::tuple<typename after_compare::second_type, Rest...>>::type;

        public:
          using type = decltype(std::tuple_cat(
              std::tuple<typename after_compare::first_type>{}, rest_sorted{}));
        };
        // 执行固定次数的冒泡排序
        template <std::size_t RemainingPasses, typename CurrentTuple>
        struct bubble_sort_impl
        {
        private:
          using after_pass =
              typename bubble_pass<RemainingPasses, CurrentTuple>::type;

        public:
          using type =
              typename bubble_sort_impl<RemainingPasses - 1, after_pass>::type;
        };
        template <typename CurrentTuple>
        struct bubble_sort_impl<0, CurrentTuple>
        {
          typedef CurrentTuple type;
        };

      public:
        using type = typename bubble_sort_impl<std::tuple_size<Tuple>::value, Tuple>::type;
      };
    } // namespace __impl_bubble
    using __impl_bubble::sort_bubble;
    namespace __impl_counting
    {
      template <typename Tuple, template <typename, typename> class Comparator>
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
          using is_true = std::integral_constant<bool, Index != _Index &&
                                                           Comparator<_T, T>::value>;
          using trues = typename filter_include<is_true, CurTuple>::type;

        public:
          using type = entry<T, std::tuple_size<trues>::value>;
        };
        // 递归构建最终元组
        template <std::size_t Rank, typename CurEntries>
        class build
        {
          template <typename T, std::size_t, typename>
          using is_equal = std::integral_constant<bool, (Rank == T::value)>;
          // 分离具有指定计数的元素
          using splited = typename filter<is_equal, CurEntries>::type;
          template <typename T, std::size_t, typename>
          using extract_type = as_type<typename T::type>;
          using first_type =
              typename map<extract_type, typename splited::first_type>::type;

        public:
          using type = typename concat<
              first_type,
              typename build<Rank + 1, typename splited::second_type>::type>::type;
        };
        template <std::size_t Rank>
        class build<Rank, std::tuple<>>
        {
        public:
          using type = std::tuple<>;
        };

      public:
        using type =
            typename build<0, typename map<count_smaller, Tuple>::type>::type;
      };
    } // namespace __impl_counting
    using __impl_counting::sort_count;
    namespace __impl_group
    {
      // 主排序模板
      template <typename Tuple, template <typename, typename> class Comparator>
      struct sort_group
      {
      private:
        template <typename CurTuple, typename OtherTuple>
        class find_min_type;
        template <typename First, typename Second, typename... Ts1, typename... Ts2>
        class find_min_type<std::tuple<First, Second, Ts1...>, std::tuple<Ts2...>>
        {
          using cmp = Comparator<typename std::tuple_element<0, First>::type,
                                 typename std::tuple_element<0, Second>::type>;
          using cmp_result =
              typename std::conditional<cmp::value, std::pair<First, Second>,
                                        std::pair<Second, First>>::type;

        public:
          using type = typename find_min_type<
              std::tuple<typename cmp_result::first_type, Ts1...>,
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
          using type = typename concat<
              typename splited::first_type,
              typename build<typename splited::second_type>::type>::type;
        };
        template <>
        class build<std::tuple<>>
        {
        public:
          using type = std::tuple<>;
        };

      public:
        using type = typename build<typename __::unique<Tuple, Comparator>::type>::type;
      };
    } // namespace __impl_group
    using __impl_group::sort_group;
    namespace __impl_merge
    {
      // 拆分类型列表为两半（交替拆分）
      template <typename List>
      struct split;
      template <>
      struct split<std::tuple<>>
      {
        using first = std::tuple<>;
        using second = std::tuple<>;
      };
      template <typename T>
      struct split<std::tuple<T>>
      {
        using first = std::tuple<T>;
        using second = std::tuple<>;
      };
      template <typename T1, typename T2, typename... Ts>
      struct split<std::tuple<T1, T2, Ts...>>
      {
      private:
        using RestSplit = split<std::tuple<Ts...>>;

      public:
        using first =
            typename concat<std::tuple<T1>, typename RestSplit::second>::type;
        using second =
            typename concat<std::tuple<T2>, typename RestSplit::first>::type;
      };
      template <typename List1, typename List2,
                template <typename, typename> class Comparator>
      class list_merge
      {
        template <typename L1, typename L2>
        struct helper;
        template <>
        struct helper<std::tuple<>, std::tuple<>>
        {
          using type = std::tuple<>;
        };
        template <typename... Ts>
        struct helper<std::tuple<Ts...>, std::tuple<>>
        {
          using type = std::tuple<Ts...>;
        };
        template <typename... Ts>
        struct helper<std::tuple<>, std::tuple<Ts...>>
        {
          using type = std::tuple<Ts...>;
        };
        template <typename Head1, typename... Tail1, typename Head2,
                  typename... Tail2>
        struct helper<std::tuple<Head1, Tail1...>, std::tuple<Head2, Tail2...>>
        {
          using type = typename std::conditional<
              Comparator<Head1, Head2>::value,
              typename concat<
                  std::tuple<Head1>,
                  typename helper<std::tuple<Tail1...>,
                                  std::tuple<Head2, Tail2...>>::type>::type,
              typename concat<std::tuple<Head2>,
                              typename helper<std::tuple<Head1, Tail1...>,
                                              std::tuple<Tail2...>>::type>::type>::
              type;
        };

      public:
        using type = typename helper<List1, List2>::type;
      };
      // 归并排序
      template <typename Tuple, template <typename, typename> class Comparator>
      class sort_merge
      {
        template <typename L, typename R>
        using cmp = Comparator<typename std::tuple_element<0, L>::type, typename std::tuple_element<0, R>::type>;
        template <typename List>
        struct helper;
        template <>
        struct helper<std::tuple<>>
        {
          using type = std::tuple<>;
        };
        template <typename T>
        struct helper<std::tuple<T>>
        {
          using type = std::tuple<T>;
        };
        template <typename... Ts>
        struct helper<std::tuple<Ts...>>
        {
        private:
          using SplitLists = split<std::tuple<Ts...>>;
          using SortedFirst = typename helper<typename SplitLists::first>::type;
          using SortedSecond = typename helper<typename SplitLists::second>::type;

        public:
          using type =
              typename list_merge<SortedFirst, SortedSecond, cmp>::type;
        };

      public:
        using type = typename transfer<concat,
                                       typename helper<typename __::unique<Tuple, Comparator>::type>::type>::type::type;
      };
    } // namespace __impl_merge
    using __impl_merge::sort_merge;
    namespace __impl_is_sorted
    {
      /*
              `type`: 判断元组是否有序
              - `Predicate`: 模板参数,其实例满足约定`type`
              - `Tuple`: 类型参数,原集合
       */
      template <typename Tuple,
                template <typename, typename> class /* value */ Comparator>
      class is_sorted
      {
        template <typename L, typename R>
        using cmp = typename ComparatorAdapter<Comparator>::template less_equal<L, R>;
        template <typename CurTuple>
        struct helper;
        template <typename First, typename Second>
        struct helper<std::tuple<First, Second>> : cmp<First, Second>
        {
        };
        template <typename First, typename Second, typename... Ts>
        struct helper<std::tuple<First, Second, Ts...>>
            : conjunction<cmp<First, Second>, helper<std::tuple<Second, Ts...>>>
        {
        };

      public:
        static const bool value = helper<Tuple>::value;
      };
    } // namespace __impl_is_sorted
    using __impl_is_sorted::is_sorted;
    namespace __impl_heap
    {
      // 堆排序
      template <typename Tuple, template <typename, typename> class Comparator>
      class sort_heap
      {
        template <typename L, typename R>
        using cmp = typename ComparatorAdapter<Comparator>::template less<L, R>;
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
          using is_small = cmp<_T, T>;
          using splited = typename filter<is_small, std::tuple<Ts...>>::type;

        public:
          using type = typename concat<
              typename helper<typename splited::first_type>::type, std::tuple<T>,
              typename helper<typename splited::second_type>::type>::type;
        };

      public:
        using type = typename helper<Tuple>::type;
      };
    } // namespace __impl_heap
    using __impl_heap::sort_heap;
  } // namespace sort
} // namespace types
#endif
