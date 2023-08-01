#include "meta_func.h"

#include <iomanip>
#include <iostream>
#include <chrono>

using namespace std::string_literals;
using namespace std::chrono_literals;

int main(int argc, char** argv)
{
    std::cout << "meta::add<1, 2>::value = "
              << meta::add<1, 2>::value << std::endl;

    struct st_foo
    {
        void foo() {}
    };

    constexpr auto int_has_foo = meta::has_member_func_foo<int>::value;
    std::cout << "int " << (int_has_foo ? "" : "not")
              << " have member foo" << std::endl;

    constexpr auto st_has_foo = meta::has_member_func_foo<st_foo>::value;
    std::cout << "st_foo " << (st_has_foo ? "" : "not")
              << " have member foo" << std::endl;

    // SFINAE(substitude failure is not an error)，
    // 可以通过enable_if来实现编译期的if-else逻辑
    std::cout << "meta::call_func(1) = " << meta::call_func(1) << std::endl;
    std::cout << "meta::call_func('lemon's) = "
              << meta::call_func("lemon"s) << std::endl;

    auto str = "1"s;
    auto xx = 1s;

    auto tp = std::make_tuple(1, 2, "lemon");
    constexpr auto SIZE = std::tuple_size_v<decltype(tp)>;

    // 遍历tuple
    meta::for_each(
        tp, std::make_index_sequence<SIZE>{},
        [](auto& item, auto idx)
    {
        std::cout << "idx = " << idx << ", value = " << item << std::endl;
    });

    std::cout << "meta::IntMax_v<sizeof(int), sizeof(char*), sizeof(double), sizeof(int64_t)> = "
              << meta::IntMax_v<sizeof(int), sizeof(char*), sizeof(double), sizeof(int64_t)> << std::endl;

    std::cout << "meta::MaxAlign_v<int, char*, double, int64_t> = "
              << meta::MaxAlign_v<int, char*, double, int64_t> << std::endl;

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}
