//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/scopes/session.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>

#include "boost/di/cpp_0x/aux_/memory.hpp"

namespace boost {
namespace di {
namespace scopes {

auto new_int = []{ return new int(); };

BOOST_AUTO_TEST_CASE(create) {
    session<>::scope<int> session1;
    session<>::scope<int> session2;

    BOOST_CHECK((
        (session1.create(new_int))(type<std::shared_ptr<int>>())
        ==
        (session1.create(new_int))(type<std::shared_ptr<int>>())
    ));

    BOOST_CHECK((
        (session2.create(new_int))(type<std::shared_ptr<int>>())
        ==
        (session2.create(new_int))(type<std::shared_ptr<int>>())
    ));

    BOOST_CHECK((
        (session1.create(new_int))(type<std::shared_ptr<int>>())
        ==
        (session2.create(new_int))(type<std::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(call) {
    session<>::scope<int> session_;

    session_.call(session_entry());
    BOOST_CHECK((std::shared_ptr<int>() != (session_.create(new_int))(type<std::shared_ptr<int>>())));

    session_.call(session_exit());
    BOOST_CHECK((std::shared_ptr<int>() == (session_.create(new_int))(type<std::shared_ptr<int>>())));

    session_.call(session_entry());
    BOOST_CHECK((std::shared_ptr<int>() != (session_.create(new_int))(type<std::shared_ptr<int>>())));

    session_.call(session_exit());
    BOOST_CHECK((std::shared_ptr<int>() == (session_.create(new_int))(type<std::shared_ptr<int>>())));
}

} // namespace scopes
} // namespace di
} // namespace boost

