//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/provider.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "common/fakes/fake_injector.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

test get_default = [] {
    fake_injector<int> injector;
    provider<int, int, no_name, aux::pair<type_traits::direct, aux::type_list<>>, decltype(injector)> provider{injector};
    std::unique_ptr<int> ptr{provider.get()};
    expect(ptr.get());
};

test get_stack = [] {
    fake_injector<int> injector;
    provider<int, int, no_name, aux::pair<type_traits::direct, aux::type_list<>>, decltype(injector)> provider{injector};
    std::unique_ptr<int> ptr{provider.get(type_traits::stack{})};
    expect(ptr.get());
};

test get_heap = [] {
    fake_injector<int> injector;
    provider<int, int, no_name, aux::pair<type_traits::direct, aux::type_list<>>, decltype(injector)> provider{injector};
    std::unique_ptr<int> ptr{provider.get(type_traits::heap{})};
    expect(ptr.get());
};

}}}} // boost::di::v1::core

