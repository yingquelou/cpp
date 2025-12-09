#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP 1
#include <tuple>
/*
  说明
  - `UnaryTypeTrait`: 约定对应的类符合C++ UnaryTypeTrait
  - `type`: 约定对应的类存在公有类型成员`type`,作为结果
 */
namespace types
{
  namespace __impl_logical
  {
    namespace _1
    {
      template <typename, typename _B1, typename... _Bn>
      struct __disjunction
      {
        using type = _B1;
      };
      template <typename _B1, typename _B2, typename... _Bn>
      struct __disjunction<typename std::enable_if<!bool(_B1::value)>::type, _B1, _B2, _Bn...>
      {
        using type = typename __disjunction<void, _B2, _Bn...>::type;
      };
      template <typename, typename _B1, typename... _Bn>
      struct __conjunction
      {
        using type = _B1;
      };
      template <typename _B1, typename _B2, typename... _Bn>
      struct __conjunction<typename std::enable_if<bool(_B1::value)>::type, _B1, _B2, _Bn...>
      {
        using type = typename __conjunction<void, _B2, _Bn...>::type;
      };
      // `UnaryTypeTrait`: 析取/或
      template <typename... _Bn>
      struct disjunction : __disjunction<void, _Bn...>::type
      {
      };
      template <>
      struct disjunction<> : std::false_type
      {
      };
      // `UnaryTypeTrait`: 合取/与
      template <typename... _Bn>
      struct conjunction : __conjunction<void, _Bn...>::type
      {
      };
      template <>
      struct conjunction<> : std::true_type
      {
      };
    }
    namespace _2
    {
      // `UnaryTypeTrait`: 析取/或
      template <typename... _Bn>
      struct disjunction : std::false_type
      {
      };
      template <typename _B>
      struct disjunction<_B> : _B
      {
      };
      template <typename _B1, typename... _Bn>
      struct disjunction<_B1, _Bn...>
          : std::conditional<bool(_B1::value),
                             _B1,
                             disjunction<_Bn...>>::type
      {
      };
      // `UnaryTypeTrait`: 合取/与
      template <typename... _Bn>
      struct conjunction : std::true_type
      {
      };
      template <typename _B>
      struct conjunction<_B> : _B
      {
      };
      template <typename _B1, typename... _Bn>
      struct conjunction<_B1, _Bn...>
          : std::conditional<bool(_B1::value),
                             conjunction<_Bn...>,
                             _B1>::type
      {
      };
    }
    using _1::conjunction;
    using _1::disjunction;
    // using _2::conjunction;
    // using _2::disjunction;
  }
  using __impl_logical::conjunction;
  using __impl_logical::disjunction;
  namespace __impl_transfer
  {
    /*
      `type`: 转发模板参数
      - 例如: 欲`std::variant<...> ==> std::tuple<...>`,
        则`using type = typename transfer<std::tuple,std::variant<...>>::type`
     */
    template <template <typename...> class Container, typename Source>
    struct transfer;
    template <
        template <typename...> class Container1,
        template <typename...> class Container2,
        typename... Ts>
    struct transfer<Container1, Container2<Ts...>>
    {
      using type = Container1<Ts...>;
    };
  }
  using __impl_transfer::transfer;
  // C++11 兼容的 index_sequence
  template <std::size_t... Is>
  struct index_sequence
  {
  };
  template <std::size_t N, std::size_t... Is>
  struct make_index_sequence
  {
    using type = typename make_index_sequence<N - 1, N - 1, Is...>::type;
  };
  template <std::size_t... Is>
  struct make_index_sequence<0, Is...>
  {
    using type = index_sequence<Is...>;
  };
}
namespace types
{
  template <typename T>
  struct as_type
  {
    using type = T;
  };
  // `type`: 集合反转
  template <typename Tuple>
  class reversal
  {
    template <typename CurTuple, typename>
    struct helper
    {
      using type = CurTuple;
    };
    template <typename... Ts1,
              typename T2, typename... Ts2>
    struct helper<std::tuple<Ts1...>, std::tuple<T2, Ts2...>>
    {
      using type = typename helper<std::tuple<T2, Ts1...>, std::tuple<Ts2...>>::type;
    };

  public:
    using type = typename helper<std::tuple<>, Tuple>::type;
  };
  namespace __impl_merge
  {
    // `type`: 是std::tuple_cat的模板类版本,但更为通用,不限于std::tuple
    template <typename... Tuples>
    struct concat
    {
      using type = decltype(std::tuple_cat(typename transfer<std::tuple, Tuples>::type{}...));
    };
  }
  using __impl_merge::concat;
  /*
      `type`: 集合映射
      - `Mapper`: 模板参数,其实例满足约定`type`
        + T 元素
        + Index 对应元素索引
        + Tuple 原集合
      - `Tuple`: 类型参数,原集合
     */
  template <template <typename /* T */,
                      std::size_t /* Index */,
                      typename /* Tuple */> class /* type */ Mapper,
            typename Tuple>
  class map
  {
    template <std::size_t, typename CurTuple>
    struct helper
    {
      using type = CurTuple;
    };
    template <std::size_t Index, typename T, typename... Ts>
    struct helper<Index, std::tuple<T, Ts...>>
    {
      using type = typename concat<
          std::tuple<typename Mapper<T, Index, Tuple>::type>,
          typename helper<Index + 1, std::tuple<Ts...>>::type>::type;
    };

  public:
    using type = typename helper<0, Tuple>::type;
  };
  /*
    `type`: 过滤
    - `Predicate`的实例是一个`UnaryTypeTrait`,
    - `type`是一个`std::pair<std::tuple<...>,std::tuple<...>>`;
    `Predicate`求真者在`std::pair::first_type`中,
    `Predicate`求假者在`std::pair::second_type`中;
    当真集与假集都需要时,不要分别使用`filter_include`、`filter_exclude`.
   */
  template <template <typename /* T */,
                      std::size_t /* Index */,
                      typename /* Tuple */> class /* value */ Predicate,
            typename Tuple>
  class filter
  {
    template <typename T,
              std::size_t Index,
              typename CurTuple>
    using pair = std::conditional<bool(Predicate<T, Index, CurTuple>::value),
                                  std::pair<std::tuple<T>, std::tuple<>>,
                                  std::pair<std::tuple<>, std::tuple<T>>>;
    using pairs = typename map<pair, Tuple>::type;
    template <typename CurTuple>
    struct splitter;
    template <typename... Ts>
    struct splitter<std::tuple<Ts...>>
    {
      using type = std::pair<std::tuple<typename Ts::first_type...>, std::tuple<typename Ts::second_type...>>;
    };
    using splitted = typename splitter<pairs>::type;

  public:
    using type = std::pair<
        typename transfer<concat, typename splitted::first_type>::type::type,
        typename transfer<concat, typename splitted::second_type>::type::type>;
  };
  // 参见`filter`
  template <template <typename /* T */,
                      std::size_t /* Index */,
                      typename /* Tuple */> class /* value */ Predicate,
            typename Tuple>
  struct filter_include
  {
    using type = typename filter<Predicate, Tuple>::type::first_type;
  };
  // 参见`filter`
  template <template <typename /* T */,
                      std::size_t /* Index */,
                      typename /* Tuple */> class /* value */ Predicate,
            typename Tuple>
  struct filter_exclude
  {
    using type = typename filter<Predicate, Tuple>::type::second_type;
  };
}
namespace types
{
  /*
    类型集合
    - 唯一性
      用types_t或types创建的集合可以保持唯一性
    - 确定性
    - 无序性
   */
  namespace __impl_in
  {
    // `UnaryTypeTrait`: 检查元素是否属于集合
    template <typename T, typename Tuple>
    struct in;
    template <typename T, typename... Ts>
    struct in<T, std::tuple<Ts...>> : disjunction<std::is_same<T, Ts>...>
    {
    };
  } // namespace __impl_in
  using __impl_in::in;
  namespace __impl_types
  {
    // 将元素添加到集合
    template <typename T,
              typename Tuple>
    class __put;
    template <
        typename T,
        typename... Ts>
    class __put<T, std::tuple<Ts...>>
    {
      using tuple = std::tuple<Ts...>;

    public:
      using type = typename std::conditional<bool(in<T, tuple>::value),
                                             tuple,
                                             std::tuple<Ts..., T>>::type;
    };
    template <typename Tuple1, typename Tuple2>
    struct __uniqueness;
    template <typename... Ts>
    struct __uniqueness<std::tuple<Ts...>, std::tuple<>>
    {
      using type = std::tuple<Ts...>;
    };
    template <typename... Ts1, typename T2, typename... Ts2>
    struct __uniqueness<std::tuple<Ts1...>, std::tuple<T2, Ts2...>>
    {
      using type = typename __uniqueness<typename __put<T2, std::tuple<Ts1...>>::type, std::tuple<Ts2...>>::type;
    };
    // `type`: 创建类型集合(会去除重复的元素)
    template <typename Tuple>
    struct types;
    template <typename... Ts>
    struct types<std::tuple<Ts...>>
    {
      using type = typename __uniqueness<std::tuple<>, std::tuple<Ts...>>::type;
    };
  }
  using __impl_types::types;
  namespace __impl_n
  {
    // 交集
    template <typename Tuple1, typename Tuple2>
    class n;
    template <
        typename... Ts1,
        typename... Ts2>
    class n<std::tuple<Ts1...>, std::tuple<Ts2...>>
    {
      template <typename T, std::size_t, typename>
      using helper = in<T, std::tuple<Ts2...>>;

    public:
      using type = typename types<typename filter_include<helper, std::tuple<Ts1...>>::type>::type;
    };
  } // namespace __impl_n
  using __impl_n::n;
  namespace __impl_x
  {
    // 差集
    template <typename Tuple1, typename Tuple2>
    class x;
    template <
        typename... Ts1,
        typename... Ts2>
    class x<std::tuple<Ts1...>, std::tuple<Ts2...>>
    {
      template <typename T,
                std::size_t Index,
                typename CurTuple>
      using helper = in<T, std::tuple<Ts2...>>;
      using filter = typename filter_exclude<helper, std::tuple<Ts1...>>::type;

    public:
      using type = typename types<filter>::type;
    };
  } // namespace __impl_x
  using __impl_x::x;
  namespace __impl_u
  {
    // 并集
    template <typename Tuple1, typename Tuple2>
    struct u;
    template <
        typename... Ts1,
        typename... Ts2>
    struct u<std::tuple<Ts1...>, std::tuple<Ts2...>>
    {
      using type = typename types<std::tuple<Ts1..., Ts2...>>::type;
    };
  } // namespace __impl_u
  using __impl_u::u;
} // namespace types
#endif
