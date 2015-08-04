//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory> // TODO
#include "boost/di/core/any_type.hpp"
#include "common/fakes/fake_injector.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

test any_type_create = [] {
    fake_injector<> injector;
    expect_eq(0, static_cast<int>(any_type<void, fake_injector<>>{injector}));
};

test any_type_ref_create = [] {
    fake_injector<> injector;
    expect_eq(0, static_cast<int>(any_type_ref<void, fake_injector<>>{injector}));
};

}}}} // boost::di::v1::core

