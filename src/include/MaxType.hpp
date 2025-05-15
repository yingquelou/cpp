#pragma once
#include <type_traits>
template <typename... T>
class MaxType
{
};
template <typename T, typename... Rest>
class MaxType<T, Rest...>
{
public:
    using type = typename MaxType<T, typename MaxType<Rest...>::type>::type;
};
template <typename T>
class MaxType<T>
{
public:
    using type = std::remove_cv_t<T>;
};

template <typename T1, typename T2>
class MaxType<T1, T2>
{
    using _T1 = std::remove_cv_t<T1>;
    using _T2 = std::remove_cv_t<T2>;

public:
    using type = std::conditional_t<(sizeof(_T1) > sizeof(_T2)), _T1, _T2>;
};
template <typename... T>
using MaxType_t = typename MaxType<T...>::type;