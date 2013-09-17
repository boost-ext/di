//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/session.hpp"

#include <boost/test/unit_test.hpp>

#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    session<>::scope<int> session1;
    session<>::scope<int> session2;

    BOOST_CHECK((
        static_cast<shared_ptr<int>>(
            session1.create())
        ==
        static_cast<shared_ptr<int>>(
            session1.create())
    ));

    BOOST_CHECK((
        static_cast<shared_ptr<int>>(
            session2.create())
        ==
        static_cast<shared_ptr<int>>(
            session2.create())
    ));

    BOOST_CHECK((
        static_cast<shared_ptr<int>>(
            session1.create())
        ==
        static_cast<shared_ptr<int>>(
            session2.create())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    session<>::scope<c2> session1;
    session<>::scope<c2> session2;

    BOOST_CHECK((
        static_cast<shared_ptr<c2>>(
            session1.create<int, double, char>(0, 0.0, '0'))
        ==
        static_cast<shared_ptr<c2>>(
            session1.create<int, double, char>(0, 0.0, '0'))
    ));

    BOOST_CHECK((
        static_cast<shared_ptr<c2>>(
            session2.create<int, double, char>(0, 0.0, '0'))
        ==
        static_cast<shared_ptr<c2>>(
            session2.create<int, double, char>(0, 0.0, '0'))
    ));

    BOOST_CHECK((
        static_cast<shared_ptr<c2>>(
            session1.create<int, double, char>(0, 0.0, '0'))
        ==
        static_cast<shared_ptr<c2>>(
            session2.create<int, double, char>(0, 0.0, '0'))
    ));
}

BOOST_AUTO_TEST_CASE(call) {
    session<>::scope<int> session_;

    session_.call(session_entry());
    BOOST_CHECK((shared_ptr<int>() != static_cast<shared_ptr<int>>(session_.create())));

    session_.call(session_exit());
    BOOST_CHECK((shared_ptr<int>() == static_cast<shared_ptr<int>>(session_.create())));

    session_.call(session_entry());
    BOOST_CHECK((shared_ptr<int>() != static_cast<shared_ptr<int>>(session_.create())));

    session_.call(session_exit());
    BOOST_CHECK((shared_ptr<int>() == static_cast<shared_ptr<int>>(session_.create())));
}

} // namespace scopes
} // namespace di
} // namespace boost

