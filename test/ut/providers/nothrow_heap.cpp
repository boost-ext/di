//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/providers/nothrow_heap.hpp"

namespace boost { namespace di { namespace providers {

test get_no_args = [] {
    auto test = [](auto type, auto init, auto memory) {
        using T = typename decltype(type)::type;
        std::unique_ptr<T> object{nothrow_heap{}.get<T>(init, memory)};
        expect(object.get());
    };

    struct c { };

    test(test_type<int>{}, type_traits::direct{}, type_traits::stack{});
    test(test_type<int>{}, type_traits::direct{}, type_traits::heap{});
    test(test_type<int>{}, type_traits::aggregate{}, type_traits::stack{});
    test(test_type<int>{}, type_traits::aggregate{}, type_traits::heap{});
    test(test_type<c>{}, type_traits::direct{}, type_traits::stack{});
    test(test_type<c>{}, type_traits::direct{}, type_traits::heap{});
    test(test_type<c>{}, type_traits::aggregate{}, type_traits::stack{});
    test(test_type<c>{}, type_traits::aggregate{}, type_traits::heap{});
};

test get_with_args = [] {
    auto test = [](auto type, auto init, auto memory, auto... args) {
        using T = typename decltype(type)::type;
        std::unique_ptr<T> object{nothrow_heap{}.get<T>(init, memory, args...)};
        expect(object.get());
    };

    struct direct { direct(int, double) { } };
    struct aggregate { int i = 0; double d = 0.0; };

    test(test_type<direct>{}, type_traits::direct{}, type_traits::stack{}, int{}, double{});
    test(test_type<direct>{}, type_traits::direct{}, type_traits::heap{}, int{}, double{});
    test(test_type<direct>{}, type_traits::aggregate{}, type_traits::stack{}, int{}, double{});
    test(test_type<direct>{}, type_traits::aggregate{}, type_traits::heap{}, int{}, double{});
    test(test_type<aggregate>{}, type_traits::aggregate{}, type_traits::stack{}, int{}, double{});
    test(test_type<aggregate>{}, type_traits::aggregate{}, type_traits::heap{}, int{}, double{});
};

}}} // boost::di::providers

