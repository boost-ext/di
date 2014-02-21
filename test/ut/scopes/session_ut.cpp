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

#include "common/fakes/fake_convertible.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    session<>::scope<int> session1;
    session<>::scope<int> session2;

    BOOST_CHECK((
        (session1.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
        ==
        (session1.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
    ));

    BOOST_CHECK((
        (session2.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
        ==
        (session2.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
    ));

    BOOST_CHECK((
        (session1.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
        ==
        (session2.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    session<>::scope<c2> session1;
    session<>::scope<c2> session2;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    BOOST_CHECK((
        (session1.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (session1.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
    ));

    BOOST_CHECK((
        (session2.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (session2.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
    ));

    BOOST_CHECK((
        (session1.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (session2.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(call) {
    session<>::scope<int> session_;

    session_.call(session_entry());
    BOOST_CHECK((aux::shared_ptr<int>() != (session_.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())));

    session_.call(session_exit());
    BOOST_CHECK((aux::shared_ptr<int>() == (session_.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())));

    session_.call(session_entry());
    BOOST_CHECK((aux::shared_ptr<int>() != (session_.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())));

    session_.call(session_exit());
    BOOST_CHECK((aux::shared_ptr<int>() == (session_.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())));
}

} // namespace scopes
} // namespace di
} // namespace boost

