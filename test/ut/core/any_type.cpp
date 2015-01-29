//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/any_type.hpp"
#include "common/fakes/fake_injector.hpp"

namespace boost { namespace di { namespace core {

test is_any_types = [] {
    expect(!is_any_type<void>{});
    expect(!is_any_type<int>{});
    expect(is_any_type<any_type<>>{});
    expect(is_any_type<any_type<int>>{});
};

test any_type_create = [] {
    fake_injector<> injector;
    expect_eq(0, static_cast<int>(any_type<void, fake_injector<>>{injector}));
};

}}} // boost::di::core

