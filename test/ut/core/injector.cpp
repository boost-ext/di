//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"
#include "boost/di/core/injector.hpp"
#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_config.hpp"

namespace boost { namespace di { namespace core {

test def_ctor = [] {
    injector<di::config> injector_{core::init{}};
    expect(std::is_same<aux::type_list<>, decltype(injector_)::deps>{});
};

test ctor = [] {
    using dep1 = fake_dependency<int>;
    using dep2 = fake_dependency<double>;
    injector<di::config, dep1, dep2> injector_{core::init{}};
};

test ctor_injector = [] {
    injector<di::config> injector1{core::init{}};
    injector<di::config> injector2{injector1};
    (void)injector2;
};

test create = [] {
    injector<di::config> injector_{core::init{}};
    expect_eq(0, injector_.create<int>());
};

test call = [] {
    fake_scope<>::entry_calls() = 0;
    fake_scope<>::exit_calls() = 0;

    using dep1 = fake_dependency<int>;
    injector<di::config, dep1> injector_{core::init{}};

    injector_.call(fake_scope_entry{});
    expect_eq(1, fake_scope<>::entry_calls());

    injector_.call(fake_scope_exit{});
    expect_eq(1, fake_scope<>::exit_calls());
};

}}} // boost::di::core

