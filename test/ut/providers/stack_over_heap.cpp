//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/providers/stack_over_heap.hpp"

namespace boost { namespace di { inline namespace v1 { namespace providers {

auto test_heap = [](auto type, auto init, auto... args) {
    using T = typename decltype(type)::type;
    std::unique_ptr<T> object{stack_over_heap{}.get<T, T>(init, type_traits::heap{}, args...)};
    expect(object.get());
};

auto test_stack = [](auto type, auto init, auto... args) {
    using T = typename decltype(type)::type;
    T object = stack_over_heap{}.get<T, T>(init, type_traits::stack{}, args...);
    (void)object;
};

test get_no_args = [] {
    struct c { };
    test_stack(test_type<int>{}, type_traits::direct{});
    test_heap(test_type<int>{}, type_traits::uniform{});
    test_stack(test_type<int>{}, type_traits::uniform{});
    test_heap(test_type<c>{}, type_traits::direct{});
    test_stack(test_type<c>{}, type_traits::direct{});
    test_heap(test_type<c>{}, type_traits::uniform{});
    test_stack(test_type<c>{}, type_traits::uniform{});
};

test get_with_args = [] {
    struct direct { direct(int, double) { } };
    struct uniform { int i = 0; double d = 0.0; };
    test_stack(test_type<direct>{}, type_traits::direct{}, int{}, double{});
    test_heap(test_type<direct>{}, type_traits::uniform{}, int{}, double{});
#if !defined(BOOST_DI_MSVC)
    test_heap(test_type<uniform>{}, type_traits::uniform{}, int{}, double{});
#endif
};

}}}} // boost::di::v1::providers

