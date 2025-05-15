#include <iostream>
#include <type_traits>
#include <tuple>
#if __cplusplus <= 201103L
namespace std
{
    template <typename...>
    struct conjunction : std::true_type
    {
    };
    template <typename T>
    struct conjunction<T> : T
    {
    };
    template <typename T, typename... Ts>
    struct conjunction<T, Ts...> : std::conditional<T::value, conjunction<Ts...>, T>::type
    {
    };
    template <typename...>
    struct disjunction : std::false_type
    {
    };
    template <typename T>
    struct disjunction<T> : T
    {
    };
    template <typename T, typename... Ts>
    struct disjunction<T, Ts...> : std::conditional<T::value, T, disjunction<Ts...>>::type
    {
    };
    template <typename T>
    struct negation : std::conditional<T::value, std::false_type, std::true_type>::type
    {
    };

} // namespace std
#endif

template <typename...>
struct type_u;
template <
    template <typename...> typename Tv1,
    typename... Ts1,
    typename... Ts2,
    typename T>
struct type_u<Tv1<Ts1...>, T, Ts2...>
{
    using type = typename std::conditional<std::disjunction<std::is_same<T, Ts1>...>::value, typename type_u<Tv1<Ts1...>, Ts2...>::type, typename type_u<Tv1<Ts1..., T>, Ts2...>::type>::type;
};
template <
    template <typename...> typename Tv,
    typename... Ts>
struct type_u<Tv<Ts...>>
{
    using type = Tv<Ts...>;
};

// 辅助函数，用于简化调用
template <typename... Args>
using UniqueTypes_t = typename type_u<std::tuple<>, Args...>::type;

// 测试示例
int main()
{
    using Types = UniqueTypes_t<int,
                                double, int, char,
                                double, int>;
    std::cout << typeid(Types).name() << std::endl; // 输出 "std::tuple<int, double, char>"
    return 0;
}
