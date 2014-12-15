//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "common/fakes/fake_assert.hpp"
#include "common/fakes/fake_data.hpp"
#include "boost/di/policies/allow_ctor_types.hpp"

namespace boost { namespace di { namespace policies {

test type_not_allowed = [] {
    try { allow_ctor_types()(fake_data<int, int>{int{}}); } catch(const assert_exception&) { };
};


}}} // boost::di::policies

