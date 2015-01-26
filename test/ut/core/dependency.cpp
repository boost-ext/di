//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/dependency.hpp"

namespace boost { namespace di { namespace core {

test is_dependency_types = [] {
    expect(!is_dependency<void>{});
    expect(!is_dependency<int>{});
    expect(is_dependency<dependency<scopes::deduce, int>>{});
    expect(is_dependency<dependency<scopes::deduce, double, double>>{});
};

struct fake_scope {
    static constexpr auto priority = false;

    template<class T, class>
    struct scope {
        explicit scope(T object, int injector = 0)
            : object(object), injector(injector)
        { }

        T object;
        int injector = 0;
    };
};

struct name { };

test types = [] {
    using dep = dependency<fake_scope, int, double, name>;
    expect(std::is_same<fake_scope, typename dep::scope>{});
    expect(std::is_same<int, typename dep::expected>{});
    expect(std::is_same<double, typename dep::given>{});
    expect(std::is_same<name, typename dep::name>{});
};

test def_ctor = [] {
    dependency<scopes::deduce, int> dep;
    (void)dep;
};

test ctor = [] {
    constexpr auto i = 42;
    dependency<fake_scope, int> dep{i};
    expect_eq(i, dep.object);
};

test named = [] {
    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<no_name, typename dep1::name>{});

    using dep2 = decltype(dep1{}.named(name{}));
    expect(std::is_same<name, typename dep2::name>{});
};

test in = [] {
    using dep1 = dependency<fake_scope, int>;
    expect(std::is_same<fake_scope, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.in(scopes::deduce{}));
    expect(std::is_same<scopes::deduce, typename dep2::scope>{});
};

test to = [] {
    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<scopes::deduce, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.to(42));
    expect(std::is_same<scopes::external, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<int, typename dep2::given>{});

    int i = 42;
    using dep3 = decltype(dep1{}.to(i));
    expect(std::is_same<scopes::external, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<int, typename dep2::given>{});
};

struct fake_injector {
    using deps = void;
    template<class T> T create_impl() const noexcept { return {}; }
};

test to_with_configure = [] {
    struct module {
        fake_injector configure() const noexcept { return {}; }
    };

    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<scopes::deduce, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.to(module{}));
    expect(std::is_same<scopes::exposed<scopes::deduce>, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<fake_injector, typename dep2::given>{});
};

test to_with_deps = [] {
    using dep1 = dependency<scopes::deduce, int>;
    expect(std::is_same<scopes::deduce, typename dep1::scope>{});

    using dep2 = decltype(dep1{}.to(fake_injector{}));
    expect(std::is_same<scopes::exposed<scopes::deduce>, typename dep2::scope>{});
    expect(std::is_same<int, typename dep2::expected>{});
    expect(std::is_same<fake_injector, typename dep2::given>{});
};

}}} // boost::di::core

