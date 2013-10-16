//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/per_thread.hpp"

#include <boost/test/unit_test.hpp>
#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"

#include "fake_convertible.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create_the_same_thread_per_request) {
    per_thread<per_request<>>::scope<int> per_thread_;

    BOOST_CHECK((
        (per_thread_.create())(type<shared_ptr<int>>())
        !=
        (per_thread_.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_thread_per_request_args) {
    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    per_thread<per_request<>>::scope<c2> per_thread_;

    BOOST_CHECK((
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        !=
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_thread_singleton) {
    per_thread<singleton<>>::scope<int> per_thread_;

    BOOST_CHECK((
        (per_thread_.create())(type<shared_ptr<int>>())
        ==
        (per_thread_.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_thread_singleton_args) {
    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    per_thread<singleton<>>::scope<c2> per_thread_;

    BOOST_CHECK((
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        ==
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}

#if 0
BOOST_AUTO_TEST_CASE(create_different_thread_per_request) {
    std::vector<thread::id> ids = { thread::id(), thread::id() };
    auto index = 0;
    per_thread<per_request<>>::scope<int> per_thread_([&]{ return ids[index++]; });

    BOOST_CHECK((
        (per_thread_.create())(type<shared_ptr<int>>())
        !=
        (per_thread_.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_different_thread_singleton) {
    std::vector<thread::id> ids = { thread::id(), thread::id() };
    auto index = 0;
    per_thread<singleton<>>::scope<int> per_thread_([&]{ return ids[index++]; });

    BOOST_CHECK((
        (per_thread_.create())(type<shared_ptr<int>>())
        !=
        (per_thread_.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_different_thread_per_request_args) {
    std::vector<thread::id> ids = { thread::id(), thread::id() };
    auto index = 0;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    per_thread<per_request<>>::scope<c2> per_thread_([&]{ return ids[index++]; });

    BOOST_CHECK((
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        !=
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_different_thread_singleton_args) {
    std::vector<thread::id> ids = { thread::id(), thread::id() };
    auto index = 0;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    per_thread<per_request<>>::scope<c2> per_thread_([&]{ return ids[index++]; });

    BOOST_CHECK((
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        !=
        (per_thread_.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}
#endif

} // namespace scopes
} // namespace di
} // namespace boost

