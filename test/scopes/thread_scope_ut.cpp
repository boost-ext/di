//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/thread.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/shared.hpp"

#include "fake_convertible.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create_the_same_thread_unique) {
    thread<unique<>>::scope<int> thread;

    BOOST_CHECK((
        (thread.create())(type<aux::shared_ptr<int>>())
        !=
        (thread.create())(type<aux::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_threadunique_args) {
    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    thread<unique<>>::scope<c2> thread;

    BOOST_CHECK((
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
        !=
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_thread_shared) {
    thread<shared<>>::scope<int> thread;

    BOOST_CHECK((
        (thread.create())(type<aux::shared_ptr<int>>())
        ==
        (thread.create())(type<aux::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_the_same_thread_shared_args) {
    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    thread<shared<>>::scope<c2> thread;

    BOOST_CHECK((
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (thread.create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>())
    ));
}

using different_thread_scopes_types = mpl::vector<
    unique<>
  , shared<>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(create_different_thread_scope, TScope, different_thread_scopes_types) {
    std::vector<aux::shared_ptr<int>> v;
    aux::mutex m;
    typename thread<TScope>::template scope<int> thread;

    aux::thread t1([&]{
        aux::scoped_lock l(m);
        (void)l;
        v.push_back((thread.create())(type<aux::shared_ptr<int>>()));
    });

    aux::thread t2([&]{
        aux::scoped_lock l(m);
        (void)l;
        v.push_back((thread.create())(type<aux::shared_ptr<int>>()));
    });

    t1.join();
    t2.join();

    BOOST_CHECK_EQUAL(2u, v.size());
    BOOST_CHECK(v[0] != v[1]);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(create_different_thread_scope_args, TScope, different_thread_scopes_types) {
    std::vector<aux::shared_ptr<c2>> v;
    aux::mutex m;
    typename thread<TScope>::template scope<c2> thread;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    aux::thread t1([&]{
        aux::scoped_lock l(m);
        (void)l;
        v.push_back((thread.template create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>()));
    });

    aux::thread t2([&]{
        aux::scoped_lock l(m);
        (void)l;
        v.push_back((thread.template create<decltype(i), decltype(d), decltype(c)>(i, d, c))(type<aux::shared_ptr<c2>>()));
    });

    t1.join();
    t2.join();

    BOOST_CHECK_EQUAL(2u, v.size());
    BOOST_CHECK(v[0] != v[1]);
}

} // namespace scopes
} // namespace di
} // namespace boost

