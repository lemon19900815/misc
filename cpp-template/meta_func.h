#pragma once

#include <type_traits>
#include <string>
#include <iostream>
#include <tuple>

// reference: https://www.cnblogs.com/qicosmos/p/4480460.html

namespace meta
{

template<int N, int M>
struct add
{
    static constexpr auto value = N + M;
};

template<typename T>
struct has_member_func_foo
{
private:
    template<typename U>
    static constexpr auto check(U*) -> decltype(std::declval<U>().foo(), std::true_type{});

    template<typename>
    static constexpr std::false_type check(...);

public:
    static constexpr bool value = decltype(check<T>(nullptr))::value;
};

template<typename T>
constexpr bool has_member_func_foo_v = has_member_func_foo<T>::value;

template<class T>
typename std::enable_if_t<std::is_arithmetic_v<T>, int> call_func(T t)
{
    //std::cout << "call_func(T t)" << std::endl;
    return 0;
}

template<class T>
typename std::enable_if_t<std::is_same_v<T, std::string>, int> call_func(T&& t)
{
    //std::cout << "call_func(T&& t)" << std::endl;
    return 1;
}

// tuple，c++17
template<typename...Args, typename F, size_t... Idx>
void for_each(const std::tuple<Args...>& tp, std::index_sequence<Idx...>, F&& f)
{
    (std::forward<F>(f)(std::get<Idx>(tp), std::integral_constant<size_t, Idx> {}),...);
}

// 获取整数序列的最大值

// 模板声明
template<size_t v, size_t... rest>
struct IntMax;

// 递归调用结束条件
template<size_t v>
struct IntMax<v> : std::integral_constant<size_t, v> { };

// template alias
template<size_t v, size_t... rest>
constexpr int IntMax_v = IntMax<v, rest...>::value;

// 模板实现
template<size_t v1, size_t v2, size_t... rest>
struct IntMax<v1, v2, rest...> : std::integral_constant<size_t,
    v1 >= v2 ? IntMax_v<v1, rest...> : IntMax_v<v2, rest...>> {};

// 内存对齐
template<typename...Args>
struct MaxAlign : std::integral_constant<size_t, IntMax_v<sizeof(Args)...>> {};

template<typename...Args>
constexpr auto MaxAlign_v = MaxAlign<Args...>::value;

} // namespace meta
