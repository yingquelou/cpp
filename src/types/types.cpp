#include "types.hpp"
#include <iostream>
#include <utility>
template <typename L, typename R>
using size_less = std::integral_constant<bool, sizeof(L) <= sizeof(R)>;
template <typename L, typename R>
using size_greater = size_less<R, L>;
void print(std::tuple<>)
{
    std::cout << std::endl;
}
template <typename T, typename... Ts>
void print(const std::tuple<T, Ts...>&)
{
    std::cout << sizeof(T) << ':' << typeid(T).name() << '\t';
    print(std::tuple<Ts...> {});
}
template <typename T, std::size_t Index, typename Tuple>
struct AddPoint {
    using type = std::integral_constant<std::size_t, Index>;
};
template <typename T, std::size_t Index, typename Tuple>
using GreaterIntSize = std::integral_constant<bool, (sizeof(T) > sizeof(int))>;
template <typename... Ts>
struct df {
    using type = types::index_sequence<Ts::value...>;
};
int main(int argc, char const* argv[])
{
    using A = std::tuple<double,
        void*,
        bool,
        unsigned short,
        short,
        unsigned char,
        char,
        unsigned int,
        long,
        unsigned long,
        long long,
        unsigned long long,
        long double,
        int,
        float>;
    using B = std::tuple<double,
        void*,
        bool,
        unsigned short,
        long double,
        int,
        float>;
    // 测试:交集
    using n = typename types::n<A, B>::type;
    std::cout << "n:" << typeid(n).name() << std::endl;
    // 测试:差集
    using x = typename types::x<A, B>::type;
    std::cout << "x:" << typeid(x).name() << std::endl;
    // 测试:并集
    using u = typename types::u<A, B>::type;
    std::cout << "u:" << typeid(u).name() << std::endl;
    // 测试:映射
    using m = typename types::map<AddPoint, A>::type;
    using ns = typename types::transfer<df, m>::type::type;
    using seq = types::make_index_sequence<1000>;

    std::cout << "map:" << typeid(seq).name() << std::endl;
    // 测试:过滤
    using fi = typename types::filter<GreaterIntSize, A>::type;
    std::cout << "filter_include:" << typeid(fi::first_type).name() << std::endl;
    std::cout << "filter_exclude:" << typeid(fi::second_type).name() << std::endl;
    return 0;
}
