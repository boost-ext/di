//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "common/fakes/fake_assert.hpp"
#include "common/fakes/fake_policy.hpp"
#include <type_traits>
#include "boost/di/policies/allow_ctor_types.hpp"

namespace boost { namespace di { namespace policies {

test nothing_is_allowed = [] {
    struct c { };
    try { allow_ctor_types()(fake_policy<int>{}); } catch(const assert_exception&) { };
    try { allow_ctor_types()(fake_policy<double>{}); } catch(const assert_exception&) { };
    try { allow_ctor_types()(fake_policy<c>{}); } catch(const assert_exception&) { };
};

test type_is_allowed = [] {
    auto test = [](auto type) {
        allow_ctor_types(std::is_same<decltype(type), _>{})(fake_policy<decltype(type)>{});
    };

    struct c { };
    test(int{});
    test(float{});
    test(c{});
};

test type_is_not_allowed = [] {
    auto test = [](auto type) {
         try { allow_ctor_types(std::is_same<decltype(type), _>{})(fake_policy<void>{}); } catch(const assert_exception&) { }
    };

    struct c { };
    test(int{});
    test(float{});
    test(c{});
};

test operator_not = [] {
    auto test_okay = [](auto type, auto allowed) {
        using namespace operators;
        allow_ctor_types(!std::is_same<_, decltype(type)>{})(fake_policy<decltype(allowed)>{});
    };

    auto test_throw = [](auto type, auto allowed) {
        using namespace operators;
        try { allow_ctor_types(
            !std::is_same<_, decltype(type)>{})(fake_policy<decltype(allowed)>{});
        } catch(const assert_exception&) { }
    };

    struct c { };
    test_okay(double{}, int{});
    test_throw(c{}, c{});
    test_throw(int{}, int{});
    test_throw(double{}, double{});
};

test operator_or = [] {
    auto test_okay = [](auto type1, auto type2, auto allowed) {
        using namespace operators;
        allow_ctor_types(std::is_same<decltype(type1), _>{} || std::is_same<_, decltype(type2)>{})(fake_policy<decltype(allowed)>{});
    };

    auto test_throw = [](auto type1, auto type2, auto allowed) {
        using namespace operators;
        try { allow_ctor_types(
            std::is_same<decltype(type1), _>{} || std::is_same<_, decltype(type2)>{})(fake_policy<decltype(allowed)>{});
        } catch(const assert_exception&) { }
    };

    test_okay(int{}, double{}, int{});
    test_okay(double{}, int{}, int{});
    test_throw(int{}, double{}, float{});
    test_throw(double{}, int{}, float{});
};

test operator_and = [] {
    auto test_okay = [](auto type, auto allowed) {
        using namespace operators;
        allow_ctor_types(std::is_integral<_>{} && std::is_same<_, decltype(type)>{})(fake_policy<decltype(allowed)>{});
    };

    auto test_throw = [](auto type, auto allowed) {
        using namespace operators;
        try { allow_ctor_types(
            std::is_integral<_>{} && std::is_same<_, decltype(type)>{})(fake_policy<decltype(allowed)>{});
        } catch(const assert_exception&) { }
    };

    struct c { };
    test_okay(int{}, int{});
    test_throw(c{}, c{});
    test_throw(int{}, float{});
    test_throw(double{}, float{});
};

test is_type_bound = [] {
    allow_ctor_types(is_bound<_>{})(fake_policy<void, aux::none_t, aux::none_t, true>{});
    try { allow_ctor_types(is_bound<_>{})(fake_policy<void, aux::none_t, aux::none_t, false>{}); } catch(const assert_exception&) { }
};

test complex_opeartors = [] {
    using namespace operators;
    auto test = [](auto data) { allow_ctor_types((std::is_integral<_>{} && std::is_same<_, int>{}) || is_bound<_>{})(data); };
    try { test(fake_policy<void, aux::none_t, aux::none_t, false>{}); } catch(const assert_exception&) { }
    try { test(fake_policy<double, aux::none_t, aux::none_t, false>{}); } catch(const assert_exception&) { }
    test(fake_policy<int, aux::none_t, aux::none_t, false>{});
    test(fake_policy<int, aux::none_t, aux::none_t, true>{});
    test(fake_policy<double, aux::none_t, aux::none_t, true>{});
};

}}} // boost::di::policies

