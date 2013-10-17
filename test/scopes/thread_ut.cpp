//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/thread.hpp"

#include <boost/test/unit_test.hpp>
#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/shared.hpp"

#include "fake_convertible.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create_the_same_threadunique) {
    thread<unique<>>::scope<int> thread;

    BOOST_CHECK((
        (thread.create())(type<shared_ptr<int>>())
        !=
        (thread.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_threadunique_args) {
    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    thread<unique<>>::scope<c2> thread;

    BOOST_CHECK((
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        !=
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_threadshared) {
    thread<shared<>>::scope<int> thread;

    BOOST_CHECK((
        (thread.create())(type<shared_ptr<int>>())
        ==
        (thread.create())(type<shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_threadshared_args) {
    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    thread<shared<>>::scope<c2> thread;

    BOOST_CHECK((
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        ==
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_different_threadunique) {
    di::thread t1([]{});
    di::thread t2([]{});
    std::vector<di::thread::id> ids = { t1.get_id(), t2.get_id() };
    auto index = 0;
    thread<unique<>>::scope<int> thread([&]{ return ids[index++]; });

    BOOST_CHECK((
        (thread.create())(type<shared_ptr<int>>())
        !=
        (thread.create())(type<shared_ptr<int>>())
    ));
    t1.join();
    t2.join();
}

BOOST_AUTO_TEST_CASE(create_different_threadshared) {
    di::thread t1([]{});
    di::thread t2([]{});
    std::vector<di::thread::id> ids = { t1.get_id(), t2.get_id() };
    auto index = 0;
    thread<shared<>>::scope<int> thread([&]{ return ids[index++]; });

    BOOST_CHECK((
        (thread.create())(type<shared_ptr<int>>())
        !=
        (thread.create())(type<shared_ptr<int>>())
    ));
    t1.join();
    t2.join();
}

BOOST_AUTO_TEST_CASE(create_different_threadunique_args) {
    di::thread t1([]{});
    di::thread t2([]{});
    std::vector<di::thread::id> ids = { t1.get_id(), t2.get_id() };
    auto index = 0;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    thread<unique<>>::scope<c2> thread([&]{ return ids[index++]; });

    BOOST_CHECK((
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        !=
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
    t1.join();
    t2.join();
}

BOOST_AUTO_TEST_CASE(create_different_threadshared_args) {
    di::thread t1([]{});
    di::thread t2([]{});
    std::vector<di::thread::id> ids = { t1.get_id(), t2.get_id() };
    auto index = 0;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    thread<unique<>>::scope<c2> thread([&]{ return ids[index++]; });

    BOOST_CHECK((
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
        !=
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<shared_ptr<c2>>())
    ));
    t1.join();
    t2.join();
}

} // namespace scopes
} // namespace di
} // namespace boost

