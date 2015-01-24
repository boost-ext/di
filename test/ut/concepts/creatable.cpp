//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/policies/constructible.hpp"
#include "boost/di/make_injector.hpp"
#include "boost/di/bindings.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di { namespace concepts {

struct i1 { virtual ~i1() = default; virtual void dummy() = 0; };
struct impl1 : public i1 { virtual void dummy() { }; };

template<
    class T
  , class TDeps = core::pool<>
  , class TPolicies = decltype(di::make_policies())
> decltype(creatable(std::declval<T>(), std::declval<TDeps>(), std::declval<TPolicies>())) creatable_v{};

test empty = [] {
    auto injector = di::make_injector();
    static_assert(creatable_v<int, decltype(injector)::deps>, "");
};

test inject_fail = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(); c3(int) { } };
    struct c2 { BOOST_DI_INJECT(c2, c3) { } };
    struct c1 { BOOST_DI_INJECT(c1, c2, c3) { } };
    static_assert(!creatable_v<c1>, "");
};

test inject_pass = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(int); c3(int) { } };
    struct c2 { BOOST_DI_INJECT(c2, c3) { } };
    struct c1 { BOOST_DI_INJECT(c1, c2, c3) { } };
    static_assert(creatable_v<c1>, "");
};

test automatic = [] {
    struct c {
        c(int, double) { }
    };
    static_assert(creatable_v<c>, "");
};

test automatic_pass = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(int); c3(int) { } };
    struct c2 { c2(c3) { } };
    struct c1 { c1(c2, c3) { } };
    static_assert(creatable_v<c1>, "");
};

test automatic_fail = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(); c3(int) { } };
    struct c2 { c2(c3) { } };
    struct c1 { c1(c2, c3) { } };
    static_assert(!creatable_v<c1>, "");
};

test traits_fail = [] {
    struct c {
        BOOST_DI_INJECT_TRAITS(int);
        c(int, double) {}
    };
    static_assert(!creatable_v<c>, "");
};

test automatic_not_bound = [] {
    struct c {
        c(std::unique_ptr<i1>, int) {}
    };
    static_assert(!creatable_v<c>, "");
};

test automatic_bound = [] {
    struct c {
        c(std::unique_ptr<i1>, int) {}
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>
    );

    static_assert(creatable_v<c, decltype(injector)::deps>, "");
};

test constructible_policy_pass = [] {
    struct c3 { c3(int) { } };
    struct c2 { c2(c3) { } };
    struct c1 { c1(c2, c3) { } };

    using namespace di::policies;
    using namespace di::policies::operators;
    auto policies = di::make_policies(
        constructible(std::is_same<c1, _>{} || std::is_same<c2, _>{} || std::is_same<c3, _>{} || std::is_same<int, _>{})
    );

    auto injector = di::make_injector(
        di::bind<int>.to(42)
    );

    static_assert(creatable_v<c1, decltype(injector)::deps, decltype(policies)>,  "");
};

test constructible_policy_fail = [] {
    struct c3 { c3(int) { } };
    struct c2 { c2(c3) { } };
    struct c1 { c1(c2, c3) { } };

    using namespace di::policies;
    using namespace di::policies::operators;
    auto policies = di::make_policies(
        constructible(std::is_same<c1, _>{} || std::is_same<c2, _>{} || std::is_same<c3, _>{})
    );

    auto injector = di::make_injector();
    static_assert(false == creatable_v<c1, decltype(injector)::deps, decltype(policies)>, "");
};

test ctors = [] {
    struct c {
        BOOST_DI_INJECT(c, int a, double d)
            : a(a), d(d) { }
        c(double d, int a) : a(a), d(d) { }

        int a = 0;
        double d = 0.0;
    };

    auto injector = di::make_injector(
        di::bind<int>.to(42)
      , di::bind<double>.to(87.0)
    );

    static_assert(creatable_v<c, decltype(injector)::deps>, "");
};

}}} // boost::di::concepts

