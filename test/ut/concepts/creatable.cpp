//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_ENABLE_CREATABLE_CONCEPT
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/policies/constructible.hpp"
#include "boost/di/make_injector.hpp"
#include "boost/di/bindings.hpp"
#include "boost/di/config.hpp"
#include "common/fakes/fake_config.hpp"

namespace boost { namespace di { namespace concepts {

struct i1 { virtual ~i1() noexcept = default; virtual void dummy() = 0; };
struct impl1 : public i1 { virtual void dummy() { }; };

test empty = [] {
    auto injector = di::make_injector();
    static_expect(creatable<decltype(injector)::deps, fake_config, int>());
};

test inject_fail = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(); c3(int) { } };
    struct c2 { BOOST_DI_INJECT(c2, c3) { } };
    struct c1 { BOOST_DI_INJECT(c1, c2, c3) { } };
    static_expect(!creatable<core::pool<>, fake_config, c1>());
};

test inject_pass = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(int); c3(int) { } };
    struct c2 { BOOST_DI_INJECT(c2, c3) { } };
    struct c1 { BOOST_DI_INJECT(c1, c2, c3) { } };
    static_expect(creatable<core::pool<>, fake_config, c1>());
};

test automatic = [] {
    struct c {
        c(int, double) { }
    };
    static_expect(creatable<core::pool<>, fake_config, c>());
};

test automatic_pass = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(int); c3(int) { } };
    struct c2 { c2(c3) { } };
    struct c1 { c1(c2, c3) { } };
    static_expect(creatable<core::pool<>, fake_config, c1>());
};

test automatic_fail = [] {
    struct c3 { BOOST_DI_INJECT_TRAITS(); c3(int) { } };
    struct c2 { c2(c3) { } };
    struct c1 { c1(c2, c3) { } };
    static_expect(!creatable<core::pool<>, fake_config, c1>());
};

test traits_fail = [] {
    struct c {
        BOOST_DI_INJECT_TRAITS(int);
        c(int, double) {}
    };
    static_expect(!creatable<core::pool<>, fake_config, c>());
};

test automatic_not_bound = [] {
    struct c {
        c(std::unique_ptr<i1>, int) {}
    };
    static_expect(!creatable<core::pool<>, fake_config, c>());
};

test automatic_bound = [] {
    struct c {
        c(std::unique_ptr<i1>, int) {}
    };

    auto injector = di::make_injector(
        di::bind<i1, impl1>
    );

    static_expect(creatable<decltype(injector)::deps, fake_config, c>());
};

struct c3 { c3(int) { } };
struct c2 { c2(c3) { } };
struct c1 { c1(c2, c3) { } };

template<class>
class config : fake_config<> {
public:
    auto policies() {
        using namespace di::policies;
        using namespace di::policies::operators;

        return di::make_policies(
            constructible(std::is_same<c1, _>{} || std::is_same<c2, _>{} || std::is_same<c3, _>{} || std::is_same<int, _>{})
        );
    }
};

test constructible_policy_pass = [] {
    auto injector = di::make_injector(
        di::bind<int>.to(42)
    );

    static_expect(creatable<decltype(injector)::deps, config, c1>(),  "");
};

template<class>
class config_fail : fake_config<> {
public:
    auto policies() {
        using namespace di::policies;
        using namespace di::policies::operators;

        return di::make_policies(
            constructible(std::is_same<c1, _>{} || std::is_same<c2, _>{} || std::is_same<c3, _>{})
        );
    }
};

test constructible_policy_fail = [] {
    auto injector = di::make_injector();
    static_expect(!creatable<decltype(injector)::deps, config_fail, c1>());
};

test ctors_many = [] {
    struct c {
        BOOST_DI_INJECT(c, int a, double d)
            : a(a), d(d) { }

        c(double d, int a) : a(a), d(d) { }

        int a = 0;
        double d = 0.0;
    };

    struct c1 { };
    struct c2 {
        c2(int) { }
    };

    auto injector = di::make_injector(
        di::bind<int>.to(42)
      , di::bind<double>.to(87.0)
    );

    static_expect(creatable<decltype(injector)::deps, fake_config, c, c1, c2>());
};

}}} // boost::di::concepts

