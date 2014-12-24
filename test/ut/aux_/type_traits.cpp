//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <string>
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace aux {

BOOST_DI_HAS_TYPE(has);

test has_type = [] {
    struct a { };
    struct c { using has = void; };

    expect(has_has<c>{});
    expect(!has_has<void>{});
    expect(!has_has<a>{});
};

BOOST_DI_HAS_METHOD(call, call);

test has_method = [] {
    struct a { };
    struct call1 { void call(int); };
    struct call2 { int call(const double&); };
    struct call3 { int call(int, double = 0.0) const noexcept; };

    expect(!has_call<a, int>{});
    expect(!has_call<call1>{});
    expect(has_call<call1, int>{});
    expect(has_call<call2, double>{});
    expect(has_call<call2, const double&>{});
    expect(!has_call<call2, std::string>{});
    expect(has_call<call2, float>{}); // convertible to double
    expect(!has_call<call3>{});
    expect(has_call<call3, int>{});
    expect(has_call<call3, int, double>{});
    expect(!has_call<call3, int, double, float>{});
};

test is_braces_constructible_types = [] {
    struct c { };
    struct ctor { ctor(int) { } };
    struct ctor_def_value { ctor_def_value(int = 0) { } };
    struct ctor_def { ctor_def() = default; };
    struct agg1 { int i; };
    struct agg2 { int& i; double d; };

    expect(is_braces_constructible_t<int>{});
    expect(is_braces_constructible_t<c>{});
    expect(is_braces_constructible_t<ctor, int>{});
    expect(!is_braces_constructible_t<ctor>{});
    expect(is_braces_constructible_t<ctor_def_value>{});
    expect(is_braces_constructible_t<ctor_def_value, int>{});
    expect(is_braces_constructible_t<ctor_def>{});
    expect(!is_braces_constructible_t<ctor_def, int>{});
    expect(is_braces_constructible_t<agg1>{});
    expect(is_braces_constructible_t<agg1, int>{});
    expect(!is_braces_constructible_t<agg1, int, double>{});
    expect(is_braces_constructible_t<agg2, int&>{});
    expect(is_braces_constructible_t<agg2, int&, double>{});
};

test remove_accessors_types = [] {
    expect(std::is_same<int, remove_accessors_t<int>>{});
    expect(std::is_same<int, remove_accessors_t<int&>>{});
    expect(std::is_same<int, remove_accessors_t<int*>>{});
    expect(std::is_same<int, remove_accessors_t<const int*>>{});
};

test make_plain_types = [] {
    auto test = [] (auto type) {
        using T = decltype(type);
        expect(std::is_same<T, make_plain_t<T>>{});
        expect(std::is_same<T, make_plain_t<T*>>{});
        expect(std::is_same<T, make_plain_t<const T*>>{});
        expect(std::is_same<T, make_plain_t<const T>>{});
        expect(std::is_same<T, make_plain_t<const T&>>{});
        expect(std::is_same<T, make_plain_t<T&>>{});
        expect(std::is_same<T, make_plain_t<std::shared_ptr<T>>>{});
        expect(std::is_same<T, make_plain_t<const std::shared_ptr<T>&>>{});
        expect(std::is_same<T, make_plain_t<std::shared_ptr<T>&>>{});
        expect(std::is_same<T, make_plain_t<volatile T>>{});
        expect(std::is_same<T, make_plain_t<T&&>>{});
        expect(std::is_same<T, make_plain_t<const T&&>>{});
    };

    struct c { };
    test(c{});
    test(int{});
};

void f1() { }
int f2(int) { return {}; }
int f3(int, const double&) { return {}; }

struct c1 {
    void f1() { }
    int f2(int) { return {}; }
    int f3(int, const double&) { return {}; }
};

struct c2 {
    void f1() const { }
    int f2(int) const { return {}; }
    int f3(int, const double&) const { return {}; }
};

test function_traits_parameters_type_functions = [] {
    expect(std::is_same<void, typename function_traits<decltype(&f1)>::result_type>{});
    expect(std::is_same<type_list<>, typename function_traits<decltype(&f1)>::args>{});

    expect(std::is_same<int, typename function_traits<decltype(&f2)>::result_type>{});
    expect(std::is_same<type_list<int>, typename function_traits<decltype(&f2)>::args>{});

    expect(std::is_same<int, typename function_traits<decltype(&f3)>::result_type>{});
    expect(std::is_same<type_list<int, const double&>, typename function_traits<decltype(&f3)>::args>{});
};

test function_traits_parameters_type_methods = [] {
    expect(std::is_same<void, typename function_traits<decltype(&c1::f1)>::result_type>{});
    expect(std::is_same<c1, typename function_traits<decltype(&c1::f1)>::base_type>{});
    expect(std::is_same<type_list<>, typename function_traits<decltype(&c1::f1)>::args>{});

    expect(std::is_same<int, typename function_traits<decltype(&c1::f2)>::result_type>{});
    expect(std::is_same<c1, typename function_traits<decltype(&c1::f2)>::base_type>{});
    expect(std::is_same<type_list<int>, typename function_traits<decltype(&c1::f2)>::args>{});

    expect(std::is_same<int, typename function_traits<decltype(&c1::f3)>::result_type>{});
    expect(std::is_same<c1, typename function_traits<decltype(&c1::f3)>::base_type>{});
    expect(std::is_same<type_list<int, const double&>, typename function_traits<decltype(&c1::f3)>::args>{});
};

test function_traits_parameters_type_const_methods = [] {
    expect(std::is_same<void, typename function_traits<decltype(&c2::f1)>::result_type>{});
    expect(std::is_same<type_list<>, typename function_traits<decltype(&c2::f1)>::args>{});

    expect(std::is_same<int, typename function_traits<decltype(&c2::f2)>::result_type>{});
    expect(std::is_same<c2, typename function_traits<decltype(&c2::f2)>::base_type>{});
    expect(std::is_same<type_list<int>, typename function_traits<decltype(&c2::f2)>::args>{});

    expect(std::is_same<int, typename function_traits<decltype(&c2::f3)>::result_type>{});
    expect(std::is_same<c2, typename function_traits<decltype(&c2::f3)>::base_type>{});
    expect(std::is_same<type_list<int, const double&>, typename function_traits<decltype(&c2::f3)>::args>{});
};

}}} // boost::di::aux

