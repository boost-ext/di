//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/creators/new_creator.hpp"

#include <memory>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/inject.hpp"

#include "common/fakes/fake_wrapper.hpp"

namespace boost {
namespace di {
namespace creators {

struct empty { };

struct ctor
{
    BOOST_DI_INJECT(ctor, int i, double d) {
        BOOST_CHECK_EQUAL(42, i);
        BOOST_CHECK_EQUAL(42.0, d);
    }
};

BOOST_AUTO_TEST_CASE(create_empty) {
    aux::unique_ptr<empty> empty_(new_creator().create<empty, empty>());
    BOOST_CHECK(empty_.get());
}

BOOST_AUTO_TEST_CASE(create_ctor) {
    fake_wrapper<int> i(42);
    fake_wrapper<double> d(42.0);
    aux::unique_ptr<ctor> ctor_(new_creator().create<ctor, ctor, decltype(i), decltype(d)>(i, d));
    BOOST_CHECK(ctor_.get());
}

BOOST_AUTO_TEST_CASE(create_int_value) {
    aux::unique_ptr<int> i(new_creator().create<int, mpl::int_<42>>());
    BOOST_CHECK_EQUAL(42, *i);
}

BOOST_AUTO_TEST_CASE(create_string_value) {
    aux::unique_ptr<std::string> s(new_creator().create<std::string, mpl::string<'s'>>());
    BOOST_CHECK_EQUAL("s", *s);
}

} // namespace creators
} // namespace di
} // namespace boost

