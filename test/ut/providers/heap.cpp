//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/providers/heap.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { inline namespace v1 { namespace providers {

auto test_heap = [](auto type, auto init, auto memory, auto... args) {
    using T = typename decltype(type)::type;
    std::unique_ptr<T> object{heap{}.get<T, T>(init, memory, args...)};
    expect(object.get());
};

test get_no_args = [] {
    struct c { };
    test_heap(test_type<int>{}, type_traits::direct{}, type_traits::stack{});
    test_heap(test_type<int>{}, type_traits::direct{}, type_traits::heap{});
    test_heap(test_type<int>{}, type_traits::uniform{}, type_traits::stack{});
    test_heap(test_type<int>{}, type_traits::uniform{}, type_traits::heap{});
    test_heap(test_type<c>{}, type_traits::direct{}, type_traits::stack{});
    test_heap(test_type<c>{}, type_traits::direct{}, type_traits::heap{});
    test_heap(test_type<c>{}, type_traits::uniform{}, type_traits::stack{});
    test_heap(test_type<c>{}, type_traits::uniform{}, type_traits::heap{});
};

test get_with_args = [] {
    struct direct { direct(int, double) { } };
    struct uniform { int i = 0; double d = 0.0; };
    test_heap(test_type<direct>{}, type_traits::direct{}, type_traits::stack{}, int{}, double{});
    test_heap(test_type<direct>{}, type_traits::direct{}, type_traits::heap{}, int{}, double{});
    test_heap(test_type<direct>{}, type_traits::uniform{}, type_traits::stack{}, int{}, double{});
    test_heap(test_type<direct>{}, type_traits::uniform{}, type_traits::heap{}, int{}, double{});
#if !defined(BOOST_DI_MSVC)
    test_heap(test_type<uniform>{}, type_traits::uniform{}, type_traits::stack{}, int{}, double{});
    test_heap(test_type<uniform>{}, type_traits::uniform{}, type_traits::heap{}, int{}, double{});
#endif
};

}}}} // boost::di::v1::providers

