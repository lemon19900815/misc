#include "meta_func.h"

#include <iomanip>
#include <iostream>
#include <chrono>

using namespace std::string_literals;
using namespace std::chrono_literals;

using namespace meta;

int main(int argc, char** argv)
{
    std::cout << "add<1, 2>::value = "
              << add<1, 2>::value << std::endl;
    {
        struct st_foo
        {
            void foo() {}
        };

        constexpr auto int_has_foo = has_member_func_foo<int>::value;
        std::cout << "int " << (int_has_foo ? "" : "not")
                  << " have member foo" << std::endl;

        constexpr auto st_has_foo = has_member_func_foo<st_foo>::value;
        std::cout << "st_foo " << (st_has_foo ? "" : "not")
                  << " have member foo" << std::endl;

        // SFINAE(substitude failure is not an error)，
        // 可以通过enable_if来实现编译期的if-else逻辑
        std::cout << "call_func(1) = "
                  << call_func(1) << std::endl;

        std::cout << "call_func('lemon's) = "
                  << call_func("lemon"s) << std::endl;
    }

    {
        auto str = "1"s;
        auto xx = 1s;

        auto tp = std::make_tuple(1, 2, "lemon");
        constexpr auto SIZE = std::tuple_size_v<decltype(tp)>;

        // 遍历tuple
        for_each(
            tp, std::make_index_sequence<SIZE>{},
            [](auto& item, auto idx)
        {
            std::cout << "idx = " << idx << ", value = " << item << std::endl;
        });
    }

    {
        std::cout << "IntMax_v<sizeof(int), sizeof(char*), sizeof(double), sizeof(int64_t)> = "
                  << IntMax_v<sizeof(int), sizeof(char*), sizeof(double), sizeof(int64_t)> << std::endl;

        std::cout << "MaxAlign_v<int, char*, double, int64_t> = "
                  << MaxAlign_v<int, char*, double, int64_t> << std::endl;
    }

    {
        std::cout << "Contains_v<int, char*, double, int64_t> = "
                  << std::boolalpha
                  << Contains_v<int, char*, double, int64_t>
                  << std::endl;

        std::cout << "Contains_v<int, char*, double, int64_t, int32_t> = "
                  << std::boolalpha
                  << Contains_v<int, char*, double, int64_t, int32_t>
                  << std::endl;
    }

    {
        // 注意IndexOf模板的问题，借助Contains模板解决
        std::cout << "IndexOf_v<int, char*, double, int64_t, int32_t> = "
                  << IndexOf_v<int, char*, double, int64_t, int32_t>
                  << std::endl;

        std::cout << "IndexOf_v<int, char,char*,double,int64_t> = "
                  << IndexOf_v<int, char, char*, double, int64_t>
                  << std::endl;
    }

    {
        std::cout << "At_t<1, int, char, double, int64_t> = "
                  << typeid(At_t<1, int, char, double, int64_t>).name()
                  << std::endl;

        std::cout << "At_t<3, int, char, double, int64_t> = "
                  << typeid(At_t<3, int, char, double, int64_t>).name()
                  << std::endl;

        // 编译错误：不能匹配到索引类型，超出了参数列表的个数
        //std::cout << "At_t<4, int, char, double, int64_t> = "
        //          << typeid(At_t<4, int, char, double, int64_t>{}).name()
        //          << std::endl;
    }

    {
        // use initializer_list and () expand param list.
        print_for_each<int, double, char, int64_t>();
    }

    {
        print_args(1, 3.14, 20L, 30LL);
    }

    {
        std::function<int(int)> f = [](int a) {return a; };
        auto f2 = [](int a) {return a; };
        print_type<function_traits<int(int)>::fn_type>("function_traits<int(int)>::fn_type = ");
        print_type<function_traits<int(int)>::args<0>::type>("function_traits<int(int)>::args<0>::type = ");
        print_type<function_traits<decltype(f)>::fn_type>("function_traits<decltype(f)>::fn_type = ");
        print_type<function_traits<decltype(f2)>::fn_type>("function_traits<decltype(f2)>::fn_type = ");

        struct MyTest {
            int operator()() { return 1; }
            void foo(int ) { }
        };

        print_type<function_traits<decltype(&MyTest::foo)>::fn_type>("function_traits<decltype(&MyTest::foo) = ");
        print_type<function_traits<MyTest>::fn_type>("function_traits<decltype(MyTest) = ");
    }

    {
        struct MyTest {
            int a;
            int b;
            int c;
        };

        meta::Variant<int, double, char, int64_t> v(1);
        std::cout << "v type = " << v.Type().name() << std::endl;

        v = 20ll;
        std::cout << "v type = " << v.Type().name() << std::endl;

        v = 3.14;
        std::cout << "v type = " << v.Type().name() << std::endl;

        v = 'a';
        std::cout << "v type = " << v.Type().name() << std::endl;

        v.Visit([](int v) {
            std::cout << "int: " << v << std::endl;
        }, [](double v) {
            std::cout << "double: " << v << std::endl;
        }, [](char v) {
            std::cout << "char: " << v << std::endl;
        }, [](int64_t v) {
            std::cout << "int64_t: " << v << std::endl;
        });

        std::cout << "v.IndexOf<double> = "
                  << v.IndexOf<double>() << std::endl;
    }

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}
