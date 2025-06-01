#include <type_traits>

// 类型列表定义
template<typename... Ts>
struct TypeList {};

// TypeList辅助：prepend 和 append
// template <typename... Ts>
// struct TypeList
// {
//     template <typename T>
//     using prepend = TypeList<T, Ts...>;

//     template <typename T>
//     using append = TypeList<Ts..., T>;
// };

// 连接两个TypeList
template <typename List1, typename List2>
struct Concat;

template <typename... Ts1, typename... Ts2>
struct Concat<TypeList<Ts1...>, TypeList<Ts2...>>
{
    using type = TypeList<Ts1..., Ts2...>;
};

// 拆分类型列表为两半（交替拆分）
template <typename List>
struct Split;

template <>
struct Split<TypeList<>>
{
    using first = TypeList<>;
    using second = TypeList<>;
};

template <typename T>
struct Split<TypeList<T>>
{
    using first = TypeList<T>;
    using second = TypeList<>;
};

template <typename T1, typename T2, typename... Ts>
struct Split<TypeList<T1, T2, Ts...>>
{
private:
    using RestSplit = Split<TypeList<Ts...>>;

public:
    using first = typename Concat<TypeList<T1>, typename RestSplit::second>::type;
    using second = typename Concat<TypeList<T2>, typename RestSplit::first>::type;
};

// 默认比较器：按 sizeof 升序
template <typename T1, typename T2>
struct DefaultLess
{
    static constexpr bool value = (sizeof(T1) < sizeof(T2));
};

// 合并两个已排序的类型列表，使用比较器 Comp
template <typename List1, typename List2, template <typename, typename> class Comp>
struct Merge;

template <template <typename, typename> class Comp>
struct Merge<TypeList<>, TypeList<>, Comp>
{
    using type = TypeList<>;
};

template <typename... Ts, template <typename, typename> class Comp>
struct Merge<TypeList<Ts...>, TypeList<>, Comp>
{
    using type = TypeList<Ts...>;
};

template <typename... Ts, template <typename, typename> class Comp>
struct Merge<TypeList<>, TypeList<Ts...>, Comp>
{
    using type = TypeList<Ts...>;
};

template <typename Head1, typename... Tail1, typename Head2, typename... Tail2, template <typename, typename> class Comp>
struct Merge<TypeList<Head1, Tail1...>, TypeList<Head2, Tail2...>, Comp>
{
    static constexpr bool cond = Comp<Head1, Head2>::value;
    using type = std::conditional_t<
        cond,
        typename Concat<TypeList<Head1>, typename Merge<TypeList<Tail1...>, TypeList<Head2, Tail2...>, Comp>::type>::type,
        typename Concat<TypeList<Head2>, typename Merge<TypeList<Head1, Tail1...>, TypeList<Tail2...>, Comp>::type>::type>;
};

// 归并排序实现，支持自定义比较器
template <typename List, template <typename, typename> class Comp = DefaultLess>
struct MergeSort;

template <template <typename, typename> class Comp>
struct MergeSort<TypeList<>, Comp>
{
    using type = TypeList<>;
};

template <typename T, template <typename, typename> class Comp>
struct MergeSort<TypeList<T>, Comp>
{
    using type = TypeList<T>;
};

template <typename... Ts, template <typename, typename> class Comp>
struct MergeSort<TypeList<Ts...>, Comp>
{
private:
    using SplitLists = Split<TypeList<Ts...>>;
    using SortedFirst = typename MergeSort<typename SplitLists::first, Comp>::type;
    using SortedSecond = typename MergeSort<typename SplitLists::second, Comp>::type;

public:
    using type = typename Merge<SortedFirst, SortedSecond, Comp>::type;
};

// 测试结构体
struct A
{
    char a[10];
};
struct B
{
    char b[5];
};
struct C
{
    char c[20];
};
struct D
{
    char d[15];
};

// 自定义比较器示例：按 sizeof 降序
template <typename T1, typename T2>
struct SizeGreater
{
    static constexpr bool value = (sizeof(T1) > sizeof(T2));
};

int main()
{
    using Unsorted = TypeList<A, B, C, D,int,short,long>;

    using sp = Split<Unsorted>;
    using f = sp::first;
    using s = sp::second;
    // 默认升序排序
    using SortedAsc = MergeSort<Unsorted>::type;
    // SortedAsc 应该是 TypeList<B, A, D, C>

    // 自定义降序排序
    using SortedDesc = MergeSort<Unsorted, SizeGreater>::type;
    // SortedDesc 应该是 TypeList<C, D, A, B>
}
