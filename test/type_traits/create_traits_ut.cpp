//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/create_traits.hpp"

#include <memory>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/ctor.hpp"

#include "fake_convertible.hpp"

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct ctor
{
    BOOST_DI_CTOR(ctor, int i, double d) {
        BOOST_CHECK_EQUAL(42, i);
        BOOST_CHECK_EQUAL(42.0, d);
    }
};

struct factory
{
    int* BOOST_DI_CREATE() {
        return nullptr;
    }
};

struct factory_ext
{
    int* BOOST_DI_CREATE(int i) {
        BOOST_CHECK_EQUAL(42, i);
        return new int(i);
    }
};

BOOST_AUTO_TEST_CASE(create_empty) {
    aux::unique_ptr<empty> empty_(create_traits<empty, empty>());
    BOOST_CHECK(empty_.get());
}

BOOST_AUTO_TEST_CASE(create_ctor) {
    fake_convertible<int> i(42);
    fake_convertible<double> d(42.0);
    aux::unique_ptr<ctor> ctor_(create_traits<ctor, ctor, decltype(i), decltype(d)>(i, d));
    BOOST_CHECK(ctor_.get());
}

BOOST_AUTO_TEST_CASE(create_int_value) {
    aux::unique_ptr<int> i(create_traits<int, mpl::int_<42>>());
    BOOST_CHECK_EQUAL(42, *i);
}

BOOST_AUTO_TEST_CASE(create_string_value) {
    aux::unique_ptr<std::string> s(create_traits<std::string, mpl::string<'s'>>());
    BOOST_CHECK_EQUAL("s", *s);
}

BOOST_AUTO_TEST_CASE(create_factory) {
    aux::unique_ptr<int> factory_(create_traits<int, factory>());
    BOOST_CHECK(!factory_.get());
}

BOOST_AUTO_TEST_CASE(create_factory_ext) {
    const int expected = 42;
    fake_convertible<int> i(expected);
    aux::unique_ptr<int> factory_ext_(create_traits<int, factory_ext, decltype(i)>(i));
    BOOST_CHECK(factory_ext_.get());
    BOOST_CHECK_EQUAL(expected, *factory_ext_);
}

} // namespace type_traits
} // namespace di
} // namespace boost

