//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/singleton.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>

#include "boost/di/aux_/memory.hpp"

#include "fake_convertible.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    singleton<>::scope<int> singleton_;

    BOOST_CHECK((
        (singleton_.create())(type<shared_ptr<int>>())
        ==
        (singleton_.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    singleton<>::scope<c2> singleton_;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    BOOST_CHECK((
        (singleton_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        ==
        (singleton_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}

} // namespace scopes
} // namespace di
} // namespace boost

