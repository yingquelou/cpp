#pragma once
#include <type_traits>
#include <utility>
#ifndef TYPES_HPP
#define TYPES_HPP 1
#include "compiler_detect.h"
#include <tuple>
/*
  说明
  - `UnaryTypeTrait`: 约定对应的类符合C++ UnaryTypeTrait
  - `type`: 约定对应的类存在公有类型成员`type`,作为结果
 */
namespace types {
#if HAS_CXX17
using std::conjunction;
using std::disjunction;
#else
namespace __impl_logical {
    namespace _1 {
        template <typename, typename _B1, typename... _Bn>
        struct __disjunction {
            using type = _B1;
        };
        template <typename _B1, typename _B2, typename... _Bn>
        struct __disjunction<typename std::enable_if<!_B1::value>::type, _B1, _B2, _Bn...> {
            using type = typename __disjunction<void, _B2, _Bn...>::type;
        };
        template <typename, typename _B1, typename... _Bn>
        struct __conjunction {
            using type = _B1;
        };
        template <typename _B1, typename _B2, typename... _Bn>
        struct __conjunction<typename std::enable_if<_B1::value>::type, _B1, _B2, _Bn...> {
            using type = typename __conjunction<void, _B2, _Bn...>::type;
        };
        // `UnaryTypeTrait`: 析取/或
        template <typename... _Bn>
        struct disjunction : __disjunction<void, _Bn...>::type {
        };
        template <>
        struct disjunction<> : std::false_type {
        };
        // `UnaryTypeTrait`: 合取/与
        template <typename... _Bn>
        struct conjunction : __conjunction<void, _Bn...>::type {
        };
        template <>
        struct conjunction<> : std::true_type {
        };
    } // namespace _1
    namespace _2 {
        // `UnaryTypeTrait`: 析取/或
        template <typename... _Bn>
        struct disjunction : std::false_type {
        };
        template <typename _B>
        struct disjunction<_B> : _B {
        };
        template <typename _B1, typename... _Bn>
        struct disjunction<_B1, _Bn...> : std::conditional<_B1::value, _B1, disjunction<_Bn...>>::type {
        };
        // `UnaryTypeTrait`: 合取/与
        template <typename... _Bn>
        struct conjunction : std::true_type {
        };
        template <typename _B>
        struct conjunction<_B> : _B {
        };
        template <typename _B1, typename... _Bn>
        struct conjunction<_B1, _Bn...> : std::conditional<_B1::value, conjunction<_Bn...>, _B1>::type {
        };
    } // namespace _2
    using _1::conjunction;
    using _1::disjunction;
    // using _2::conjunction;
    // using _2::disjunction;
} // namespace __impl_logical
using __impl_logical::conjunction;
using __impl_logical::disjunction;
#endif
namespace __impl_transfer {
    /*
      `type`: 转发模板参数
      - 例如: 欲`std::variant<...> ==> std::tuple<...>`,
        则`using type = typename transfer<std::tuple,std::variant<...>>::type`
     */
    template <template <typename...> class Container, typename Source>
    struct transfer;
    template <template <typename...> class Container1, template <typename...> class Container2, typename... Ts>
    struct transfer<Container1, Container2<Ts...>> {
        using type = Container1<Ts...>;
    };
} // namespace __impl_transfer
using __impl_transfer::transfer;
#if HAS_CXX14
using std::integer_sequence;
using std::index_sequence;
using std::index_sequence_for;
using std::make_index_sequence;
using std::make_integer_sequence;
#else
// C++14标准前兼容的编译时的整数序列
template <typename T, T... Ints>
struct integer_sequence {
    using value_type = T;
    static_assert(std::is_integral<T>::value, "T must be an integral type");
    static constexpr size_t size() noexcept { return sizeof...(Ints); }
};
namespace __impl_integer_sequence {
    namespace __1 {
        template <typename T, T N>
        class __make_integer_sequence {
            template <T CurInt, T... Ints>
            struct helper {
                using type = typename helper<CurInt - 1, CurInt - 1, Ints...>::type;
            };
            template <T... Ints>
            struct helper<0, Ints...> {
                using type = integer_sequence<T, Ints...>;
            };

        public:
            using type = typename helper<N>::type;
        };
    }
    template <typename, typename>
    struct __sequence_concat;
    template <typename T, T... Is1, T... Is2>
    struct __sequence_concat<integer_sequence<T, Is1...>, integer_sequence<T, Is2...>> {
        using type = integer_sequence<T, Is1..., (Is2 + sizeof...(Is1))...>;
    };
    namespace __2 {
        template <typename T, T N>
        class __make_integer_sequence {
            template <T N1, T N2>
            struct helper {
                using type = typename __sequence_concat<typename __make_integer_sequence<T, N1>::type,
                    typename __make_integer_sequence<T, N2>::type>::type;
            };
            template <>
            struct helper<0, 0> {
                using type = integer_sequence<T>;
            };
            template <>
            struct helper<0, 1> {
                using type = integer_sequence<T, 0>;
            };

        public:
            using type = typename helper<N / 2, N - N / 2>::type;
        };
    }
    namespace __3 {
        // 主模板 - 默认不定义，强制使用特化
        template <typename T, T N, typename Enable = void>
        struct __make_integer_sequence;

        // 特化：N = 0
        template <typename T, T N>
        struct __make_integer_sequence<T, N, typename std::enable_if<N == 0>::type> {
            using type = integer_sequence<T>;
        };

        // 特化：N = 1
        template <typename T, T N>
        struct __make_integer_sequence<T, N, typename std::enable_if<N == 1>::type> {
            using type = integer_sequence<T, 0>;
        };

        // 特化：N >= 2
        template <typename T, T N>
        struct __make_integer_sequence<T, N, typename std::enable_if<(N >= 2)>::type> {
        private:
            static constexpr T half = N / 2;

            using left = typename __make_integer_sequence<T, half>::type;
            using right_base = typename __make_integer_sequence<T, N - half>::type;

        public:
            using type = typename __sequence_concat<left, right_base>::type;
        };
    }
    using __3::__make_integer_sequence;
}
template <typename T, T N>
using make_integer_sequence = typename __impl_integer_sequence::__make_integer_sequence<T, N>::type;
template <size_t... Ints>
using index_sequence = integer_sequence<size_t, Ints...>;
template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;
template <typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;
#endif
} // namespace types
namespace types {
template <typename T>
struct as_type {
    using type = T;
};
// `type`: 集合反转
template <typename Tuple>
class reversal {
    template <typename CurTuple, typename>
    struct helper {
        using type = CurTuple;
    };
    template <typename... Ts1, typename T2, typename... Ts2>
    struct helper<std::tuple<Ts1...>, std::tuple<T2, Ts2...>> {
        using type = typename helper<std::tuple<T2, Ts1...>, std::tuple<Ts2...>>::type;
    };

public:
    using type = typename helper<std::tuple<>, Tuple>::type;
};
namespace __impl_concat {
    // `type`: 是std::tuple_cat的模板类版本,但更为通用,不限于std::tuple
    template <typename... Tuples>
    struct concat {
        using type = decltype(std::tuple_cat(typename transfer<std::tuple, Tuples>::type {}...));
    };
} // namespace __impl_concat
using __impl_concat::concat;
/*
    `type`: 集合映射
    - `Mapper`: 模板参数,其实例满足约定`type`
      + T 元素
      + Index 对应元素索引
      + Tuple 原集合
    - `Tuple`: 类型参数,原集合
   */
template <template <typename /* T */, std::size_t /* Index */, typename /* Tuple */> class /* type */ Mapper,
    typename Tuple>
class map {
    template <typename, typename>
    struct helper;
    template <typename... Ts, std::size_t... Is>
    struct helper<index_sequence<Is...>, std::tuple<Ts...>> {
        using type = std::tuple<typename Mapper<Ts, Is, Tuple>::type...>;
    };

public:
    using type = typename helper<make_index_sequence<std::tuple_size<Tuple>::value>, Tuple>::type;
};
/*
  `type`: 过滤
  - `Predicate`的实例是一个`UnaryTypeTrait`,
  - `type`是一个`std::pair<std::tuple<...>,std::tuple<...>>`;
  `Predicate`求真者在`std::pair::first_type`中,
  `Predicate`求假者在`std::pair::second_type`中;
  当真集与假集都需要时,不要分别使用`filter_include`、`filter_exclude`.
 */
template <template <typename /* T */, std::size_t /* Index */, typename /* Tuple */> class /* value */ Predicate,
    typename Tuple>
class filter {
    template <typename T, std::size_t Index, typename CurTuple>
    using pair = std::conditional<Predicate<T, Index, CurTuple>::value, std::pair<std::tuple<T>, std::tuple<>>,
        std::pair<std::tuple<>, std::tuple<T>>>;
    using pairs = typename map<pair, Tuple>::type;
    template <typename CurTuple>
    struct splitter;
    template <typename... Ts>
    struct splitter<std::tuple<Ts...>> {
        using type = std::pair<std::tuple<typename Ts::first_type...>, std::tuple<typename Ts::second_type...>>;
    };
    using splitted = typename splitter<pairs>::type;

public:
    using type = std::pair<typename transfer<concat, typename splitted::first_type>::type::type,
        typename transfer<concat, typename splitted::second_type>::type::type>;
};
// 参见`filter`
template <template <typename /* T */, std::size_t /* Index */, typename /* Tuple */> class /* value */ Predicate,
    typename Tuple>
struct filter_include {
    using type = typename filter<Predicate, Tuple>::type::first_type;
};
// 参见`filter`
template <template <typename /* T */, std::size_t /* Index */, typename /* Tuple */> class /* value */ Predicate,
    typename Tuple>
struct filter_exclude {
    using type = typename filter<Predicate, Tuple>::type::second_type;
};
} // namespace types
namespace types {
/*
  类型集合
  - 唯一性
    用types_t或types创建的集合可以保持唯一性
  - 确定性
  - 无序性
 */
namespace __impl_in {
    // `UnaryTypeTrait`: 检查元素是否属于集合
    template <typename T, typename Tuple>
    struct in;
    template <typename T, typename... Ts>
    struct in<T, std::tuple<Ts...>> : disjunction<std::is_same<T, Ts>...> {
    };
} // namespace __impl_in
using __impl_in::in;
namespace __impl_types {
    template <typename Tuple, typename>
    struct __uniqueness {
        using type = Tuple;
    };
    template <typename... Ts1, typename T2, typename... Ts2>
    struct __uniqueness<std::tuple<Ts1...>, std::tuple<T2, Ts2...>> {
        using type = typename __uniqueness<
            typename std::conditional<in<T2, std::tuple<Ts1...>>::value, std::tuple<Ts1...>, std::tuple<Ts1..., T2>>::type,
            std::tuple<Ts2...>>::type;
    };
} // namespace __impl_types
// `type`: 创建类型集合(会去除重复的元素)
template <typename Tuple>
using types = __impl_types::__uniqueness<std::tuple<>, Tuple>;
namespace __impl_n {
    // 交集
    template <typename Tuple1, typename Tuple2>
    class n {
        template <typename T, std::size_t, typename>
        using helper = in<T, Tuple2>;

    public:
        using type = typename types<typename filter_include<helper, Tuple1>::type>::type;
    };
} // namespace __impl_n
using __impl_n::n;
namespace __impl_x {
    // 差集
    template <typename Tuple1, typename Tuple2>
    class x {
        template <typename T, std::size_t, typename>
        using helper = in<T, Tuple2>;

    public:
        using type = typename types<typename filter_exclude<helper, Tuple1>::type>::type;
    };
} // namespace __impl_x
using __impl_x::x;
namespace __impl_u {
    // 并集
    template <typename Tuple1, typename Tuple2>
    struct u {
        using type = typename types<typename concat<Tuple1, Tuple2>::type>::type;
    };
} // namespace __impl_u
using __impl_u::u;

} // namespace types
#endif
