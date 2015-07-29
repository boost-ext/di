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

namespace boost { namespace di { inline namespace v1 { namespace core {

struct def1 { struct creator { }; };
struct def2 { struct creator { }; };
struct ctor1 { struct creator { creator(int) {} }; };
struct ctor2 { struct creator { creator(int) {} }; };

test default_constructible = [] {
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<>>>::value);
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<def1>>>::value);
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<def1, def2>>>::value);
    static_expect(std::is_same<aux::type_list<ctor1>, copyable_t<aux::type_list<def1, ctor1, def2>>>::value);
    static_expect(std::is_same<aux::type_list<ctor1, ctor2>, copyable_t<aux::type_list<def1, ctor1, def2, ctor2>>>::value);
};

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

test conversion = [] {
    injector<di::config> injector_{core::init{}};
    expect_eq(0, (int)injector_);
};

}}}} // boost::di::v1::core

