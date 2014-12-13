//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/scopes/shared.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>

#include "boost/di/cpp_0x/aux_/memory.hpp"

namespace boost {
namespace di {
namespace scopes {

auto new_int = []{ return new int(); };

BOOST_AUTO_TEST_CASE(create) {
    shared<>::scope<int> shared_;

    BOOST_CHECK((
        (shared_.create(new_int))(type<std::shared_ptr<int>>())
        ==
        (shared_.create(new_int))(type<std::shared_ptr<int>>())
    ));
}

} // namespace scopes
} // namespace di
} // namespace boost

