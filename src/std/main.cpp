#include <type_traits>

// 类型集合模板
template <typename... Ts>
struct types {};

// __filter_impl 递归实现
template <template <typename> typename Predicate, typename Accumulated, typename Remaining>
struct __filter_impl;

// 递归情况：Remaining 非空，拆分第一个类型 T 和剩余类型 Ts...
template <template <typename> typename Predicate, typename... AccumulatedTs, typename T, typename... Ts>
struct __filter_impl<Predicate, types<AccumulatedTs...>, types<T, Ts...>> {
    using type = typename std::conditional<
        Predicate<T>::value,
        typename __filter_impl<Predicate, types<AccumulatedTs..., T>, types<Ts...>>::type,
        typename __filter_impl<Predicate, types<AccumulatedTs...>, types<Ts...>>::type
    >::type;
};

// 递归终止条件：Remaining 为空，返回累积结果
template <template <typename> typename Predicate, typename... AccumulatedTs>
struct __filter_impl<Predicate, types<AccumulatedTs...>, types<>> {
    using type = types<AccumulatedTs...>;
};

// 用户接口 filter
template <template <typename> typename Predicate, typename Types>
struct filter;

template <template <typename> typename Predicate, typename... Ts>
struct filter<Predicate, types<Ts...>> {
    using type = typename __filter_impl<Predicate, types<>, types<Ts...>>::type;
};

// 测试用例
#include <iostream>
#include <type_traits>

// 谓词：判断类型是否为整型
template <typename T>
struct is_integral_pred : std::is_integral<T> {};

int main() {
    using input_types = types<int, double, char, float, long>;
    using filtered = filter<is_integral_pred, input_types>::type;

    // 验证结果，filtered 应该是 types<int, char, long>
    std::cout << std::boolalpha;
    std::cout << std::is_same<filtered, types<int, char, long>>::value << std::endl; // 输出 true

    return 0;
}
