//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/copy.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace convertibles {

struct interface
{
    virtual ~interface() { }
    virtual void dummy() = 0;
};

struct implementation : public interface
{
    virtual void dummy() { };
};

const int i = 42;
auto return_i = []{ return new int(i); };

BOOST_AUTO_TEST_CASE(to_interface) {
    unique_ptr<interface> object =
        copy<interface>([]{ return new implementation(); });
    BOOST_CHECK(dynamic_cast<implementation*>(object.get()));
}

BOOST_AUTO_TEST_CASE(to_ptr) {
    unique_ptr<int> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_named) {
    named<int> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    shared_ptr<int> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_named_shared_ptr) {
    named<shared_ptr<int>> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_auto_ptr) {
    auto_ptr<int> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_named_auto_ptr) {
    const int i = 42;
    named<auto_ptr<int>> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_std_shared_ptr) {
    shared_ptr<int> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_named_std_shared_ptr) {
    named<shared_ptr<int>> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_unique_ptr) {
    unique_ptr<int> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_named_unique_ptr) {
    named<unique_ptr<int>> object = copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

} // namespace convertibles
} // namespace di
} // namespace boost

