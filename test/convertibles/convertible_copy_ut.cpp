//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/convertible_copy.hpp"

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
    std::unique_ptr<interface> object =
        convertible_copy<interface>([]{ return new implementation(); });
    BOOST_CHECK(dynamic_cast<implementation*>(object.get()));
}

BOOST_AUTO_TEST_CASE(to_ptr) {
    std::unique_ptr<int> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_named) {
    named<int> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, object);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    shared_ptr<int> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_named_shared_ptr) {
    named<shared_ptr<int>> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_auto_ptr) {
    std::auto_ptr<int> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

#if 0
BOOST_AUTO_TEST_CASE(to_named_auto_ptr) {
    const int i = 42;
    named<std::auto_ptr<int>> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}
#endif

BOOST_AUTO_TEST_CASE(to_std_shared_ptr) {
    std::shared_ptr<int> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

#if 0
BOOST_AUTO_TEST_CASE(to_named_std_shared_ptr) {
    named<std::unique_ptr<int>> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}
#endif

BOOST_AUTO_TEST_CASE(to_unique_ptr) {
    std::unique_ptr<int> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}

#if 0
BOOST_AUTO_TEST_CASE(to_named_unique_ptr) {
    named<std::unique_ptr<int>> object = convertible_copy<int>(return_i);
    BOOST_CHECK_EQUAL(i, *object);
}
#endif

} // namespace convertibles
} // namespace di
} // namespace boost

