//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/session.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>
#include <boost/ref.hpp>

#include "boost/di/aux_/memory.hpp"

#include "common/fakes/fake_wrapper.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    session<>::scope<int> session1;
    session<>::scope<int> session2;

    BOOST_CHECK((
        (session1.create())(type<aux::shared_ptr<int>>())
        ==
        (session1.create())(type<aux::shared_ptr<int>>())
    ));

    BOOST_CHECK((
        (session2.create())(type<aux::shared_ptr<int>>())
        ==
        (session2.create())(type<aux::shared_ptr<int>>())
    ));

    BOOST_CHECK((
        (session1.create())(type<aux::shared_ptr<int>>())
        ==
        (session2.create())(type<aux::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    session<>::scope<c2> session1;
    session<>::scope<c2> session2;

    fake_wrapper<int> i(0);
    fake_wrapper<double> d(0.0);
    fake_wrapper<char> c('0');

    BOOST_CHECK((
        (session1.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (session1.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
    ));

    BOOST_CHECK((
        (session2.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (session2.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
    ));

    BOOST_CHECK((
        (session1.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (session2.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(call) {
    session<>::scope<int> session_;

    session_.call(session_entry());
    BOOST_CHECK((aux::shared_ptr<int>() != (session_.create())(type<aux::shared_ptr<int>>())));

    session_.call(session_exit());
    BOOST_CHECK((aux::shared_ptr<int>() == (session_.create())(type<aux::shared_ptr<int>>())));

    session_.call(session_entry());
    BOOST_CHECK((aux::shared_ptr<int>() != (session_.create())(type<aux::shared_ptr<int>>())));

    session_.call(session_exit());
    BOOST_CHECK((aux::shared_ptr<int>() == (session_.create())(type<aux::shared_ptr<int>>())));
}

BOOST_AUTO_TEST_CASE(call_args) {
    session<>::scope<c2> session_;

    fake_wrapper<int> i(0);
    fake_wrapper<double> d(0.0);
    fake_wrapper<char> c('0');

    session_.call(session_entry());
    BOOST_CHECK((aux::shared_ptr<c2>() != (session_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())));

    session_.call(session_exit());
    BOOST_CHECK((aux::shared_ptr<c2>() == (session_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())));

    session_.call(session_entry());
    BOOST_CHECK((aux::shared_ptr<c2>() != (session_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())));

    session_.call(session_exit());
    BOOST_CHECK((aux::shared_ptr<c2>() == (session_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())));
}

} // namespace scopes
} // namespace di
} // namespace boost

