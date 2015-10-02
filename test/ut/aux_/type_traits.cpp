//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <string>
#include <vector>
#include <set>
#include "boost/di/aux_/type_traits.hpp"
#include "common/common.hpp"

namespace boost { namespace di { inline namespace v1 { namespace aux {

BOOST_DI_HAS_TYPE(has_has, has);

test has_type = [] {
    struct a { };
    struct c { using has = void; };

    static_expect(has_has<c>{});
    static_expect(!has_has<void>{});
    static_expect(!has_has<a>{});
};

BOOST_DI_HAS_METHOD(has_call, call);

test has_method = [] {
    struct a { };
    struct call1 { void call(int) { }; };
    struct call2 { int call(const double&) { return {}; }; };
    struct call3 { int call(int, double = 0.0) const noexcept { return {}; }; };

    static_expect(!has_call<a, int>{});
    static_expect(!has_call<call1>{});
    static_expect(has_call<call1, int>{});
    static_expect(has_call<call2, double>{});
    static_expect(has_call<call2, const double&>{});
    static_expect(!has_call<call2, std::string>{});
    static_expect(has_call<call2, float>{}); // convertible to double
    static_expect(!has_call<call3>{});
    static_expect(has_call<call3, int>{});
    static_expect(has_call<call3, int, double>{});
    static_expect(!has_call<call3, int, double, float>{});
};

test is_braces_constructible_types = [] {
    struct c { };
    struct ctor { ctor(int) { } };
    struct ctor_def_value { ctor_def_value(int = 0) { } };
    struct ctor_def { ctor_def() noexcept = default; };
    struct agg1 { int i = {}; };
    struct agg2 { int& i; double d = {}; };
    struct agg3 { int i; double d; float f; };

    static_expect(is_braces_constructible<int>{});
    static_expect(is_braces_constructible<c>{});
    static_expect(is_braces_constructible<ctor, int>{});
    static_expect(!is_braces_constructible<ctor>{});
    static_expect(is_braces_constructible<ctor_def_value>{});
    static_expect(is_braces_constructible<ctor_def_value, int>{});
    static_expect(is_braces_constructible<ctor_def>{});
    static_expect(!is_braces_constructible<ctor_def, int>{});
    static_expect(is_braces_constructible<agg1>{});

    static_expect(!is_braces_constructible<agg1, int, double>{});
#if !defined(_MSC_VER)
    static_expect(is_braces_constructible<agg1, int>{});
    static_expect(is_braces_constructible<agg2, int&>{});
    static_expect(is_braces_constructible<agg2, int&, double>{});
#endif
    static_expect(is_braces_constructible<agg3, int, double, float>{});
};

test is_narrowed_types = [] {
    static_expect(!is_narrowed<int, int>::value);
    static_expect(!is_narrowed<float, float>::value);
    static_expect(!is_narrowed<double, double>::value);
    static_expect(is_narrowed<int, double>::value);
    static_expect(is_narrowed<int, float>::value);
    static_expect(is_narrowed<float, int>::value);
    static_expect(is_narrowed<double, int>::value);
    static_expect(is_narrowed<float, double>::value);
    struct c {};
    static_expect(!is_narrowed<int, c>::value);
};

test remove_specifiers_types = [] {
    static_expect(std::is_same<int, remove_specifiers_t<int>>{});
    static_expect(std::is_same<int, remove_specifiers_t<int&>>{});
    static_expect(std::is_same<int, remove_specifiers_t<int*>>{});
    static_expect(std::is_same<int, remove_specifiers_t<const int*>>{});
};

test deref_types = [] {
    static_expect(std::is_same<typename deref_type<void>::type, void>{});
    static_expect(std::is_same<typename deref_type<int>::type, int>{});
    static_expect(std::is_same<typename deref_type<std::unique_ptr<int>>::type, int>{});
    static_expect(std::is_same<typename deref_type<std::unique_ptr<int, deleter<int>>>::type, int>{});
    static_expect(std::is_same<typename deref_type<std::shared_ptr<int>>::type, int>{});
    static_expect(std::is_same<typename deref_type<boost::shared_ptr<int>>::type, int>{});
    static_expect(std::is_same<typename deref_type<std::weak_ptr<int>>::type, int>{});
};

test decay_types = [] {
    auto test = [] (auto type) {
        using T = decltype(type);
        static_expect(std::is_same<T, decay_t<T>>{});
        static_expect(std::is_same<T, decay_t<T*>>{});
        static_expect(std::is_same<T, decay_t<const T*>>{});
        static_expect(std::is_same<T, decay_t<const T>>{});
        static_expect(std::is_same<T, decay_t<const T&>>{});
        static_expect(std::is_same<T, decay_t<T&>>{});
        static_expect(std::is_same<T, decay_t<std::shared_ptr<T>>>{});
        static_expect(std::is_same<T, decay_t<boost::shared_ptr<T>>>{});
        static_expect(std::is_same<T, decay_t<const std::shared_ptr<T>&>>{});
        static_expect(std::is_same<T, decay_t<const boost::shared_ptr<T>&>>{});
        static_expect(std::is_same<T, decay_t<std::shared_ptr<T>&>>{});
        static_expect(std::is_same<T, decay_t<boost::shared_ptr<T>&>>{});
        static_expect(std::is_same<T, decay_t<volatile T>>{});
        static_expect(std::is_same<T, decay_t<T&&>>{});
        static_expect(std::is_same<T, decay_t<const T&&>>{});
        //static_expect(std::is_same<T*[], decay_t<std::vector<std::shared_ptr<T>>>>{});
        //static_expect(std::is_same<T*[], decay_t<std::shared_ptr<std::vector<std::shared_ptr<T>>>>>{});
        //static_expect(std::is_same<T*[], decay_t<std::set<std::shared_ptr<T>>>>{});
        //static_expect(std::is_same<T*[], decay_t<std::shared_ptr<std::set<std::shared_ptr<T>>>>>{});
    };

    struct c { };
    test(c{});
    test(int{});
    test(_{});
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
    static_expect(std::is_same<void, typename function_traits<decltype(&f1)>::result_type>{});
    static_expect(std::is_same<type_list<>, typename function_traits<decltype(&f1)>::args>{});

    static_expect(std::is_same<int, typename function_traits<decltype(&f2)>::result_type>{});
    static_expect(std::is_same<type_list<int>, typename function_traits<decltype(&f2)>::args>{});

    static_expect(std::is_same<int, typename function_traits<decltype(&f3)>::result_type>{});
    static_expect(std::is_same<type_list<int, const double&>, typename function_traits<decltype(&f3)>::args>{});
};

test function_traits_parameters_type_methods = [] {
    static_expect(std::is_same<void, typename function_traits<decltype(&c1::f1)>::result_type>{});
    static_expect(std::is_same<c1, typename function_traits<decltype(&c1::f1)>::base_type>{});
    static_expect(std::is_same<type_list<>, typename function_traits<decltype(&c1::f1)>::args>{});

    static_expect(std::is_same<int, typename function_traits<decltype(&c1::f2)>::result_type>{});
    static_expect(std::is_same<c1, typename function_traits<decltype(&c1::f2)>::base_type>{});
    static_expect(std::is_same<type_list<int>, typename function_traits<decltype(&c1::f2)>::args>{});

    static_expect(std::is_same<int, typename function_traits<decltype(&c1::f3)>::result_type>{});
    static_expect(std::is_same<c1, typename function_traits<decltype(&c1::f3)>::base_type>{});
    static_expect(std::is_same<type_list<int, const double&>, typename function_traits<decltype(&c1::f3)>::args>{});
};

test function_traits_parameters_type_const_methods = [] {
    static_expect(std::is_same<void, typename function_traits<decltype(&c2::f1)>::result_type>{});
    static_expect(std::is_same<type_list<>, typename function_traits<decltype(&c2::f1)>::args>{});

    static_expect(std::is_same<int, typename function_traits<decltype(&c2::f2)>::result_type>{});
    static_expect(std::is_same<c2, typename function_traits<decltype(&c2::f2)>::base_type>{});
    static_expect(std::is_same<type_list<int>, typename function_traits<decltype(&c2::f2)>::args>{});

    static_expect(std::is_same<int, typename function_traits<decltype(&c2::f3)>::result_type>{});
    static_expect(std::is_same<c2, typename function_traits<decltype(&c2::f3)>::base_type>{});
    static_expect(std::is_same<type_list<int, const double&>, typename function_traits<decltype(&c2::f3)>::args>{});
};

class fwd;
test is_complete_types = [] {
    struct c;
    static_expect(!is_complete<c>::value);
    static_expect(!is_complete<class Fwd>::value);
    static_expect(!is_complete<fwd>::value);
    struct complete { };
    struct i { virtual ~i() = 0; };
    static_expect(is_complete<i>::value);
    static_expect(is_complete<complete>::value);
    static_expect(is_complete<int>::value);
};

}}}} // boost::di::v1::aux

