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

struct no_ctor {
    template<class...> struct scope { };
};

struct ctor {
    template<class...> struct scope { explicit scope(int) { } };
};

struct def1 { using expected = int; using given = int; using scope = no_ctor; };
struct def2 { using expected = int; using given = int; using scope = no_ctor; };
struct def_ctor1 { using expected = int; using given = int; using scope = ctor; };
struct def_ctor2 { using expected = int; using given = int; using scope = ctor; };

test copyable_types = [] {
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<>>>::value);
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<def1>>>::value);
    static_expect(std::is_same<aux::type_list<>, copyable_t<aux::type_list<def1, def2>>>::value);
    static_expect(std::is_same<aux::type_list<def_ctor1>, copyable_t<aux::type_list<def1, def_ctor1, def2>>>::value);
    static_expect(std::is_same<aux::type_list<def_ctor1, def_ctor2>, copyable_t<aux::type_list<def1, def_ctor1, def2, def_ctor2>>>::value);
};

template<class T, class TIsReferable>
using dep = fake_dependency<T, fake_scope<>, TIsReferable>;

struct c { };

test referable_types = [] {
    static_expect(std::is_same<int, referable_t<int, dep<int, std::false_type>>>::value);
    static_expect(std::is_same<int, referable_t<int, dep<int, std::true_type>>>::value);
    static_expect(std::is_same<c, referable_t<const c&, dep<c, std::false_type>>>::value);
    static_expect(std::is_same<const c&, referable_t<const c&, dep<c, std::true_type>>>::value);
    static_expect(std::is_same<c, referable_t<c&, dep<c, std::false_type>>>::value);
    static_expect(std::is_same<c&, referable_t<c&, dep<c, std::true_type>>>::value);
    static_expect(std::is_same<c, referable_t<c&, dep<c, std::false_type>>>::value);
    static_expect(std::is_same<c&&, referable_t<c&&, dep<c, std::true_type>>>::value);
    static_expect(std::is_same<int, referable_t<int&, dep<int, std::false_type>>>::value);
    static_expect(std::is_same<int&, referable_t<int&, dep<int, std::true_type>>>::value);
    static_expect(std::is_same<int, referable_t<const int&, dep<int, std::false_type>>>::value);
    static_expect(std::is_same<const int&, referable_t<const int&, dep<int, std::true_type>>>::value);
    static_expect(std::is_same<int&&, referable_t<int&&, dep<int, std::true_type>>>::value);
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
    expect(0 == injector_.create<int>());
};

test conversion = [] {
    injector<di::config> injector_{core::init{}};
    expect(0 == (int)injector_);
};

}}}} // boost::di::v1::core

