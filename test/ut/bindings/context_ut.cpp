//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/bindings/context.hpp"

#include <boost/test/unit_test.hpp>

#include "common/fakes/fake_data.hpp"
#include "common/fakes/fake_scope.hpp"

namespace boost {
namespace di {
namespace bindings {

BOOST_AUTO_TEST_CASE(empty_context) {
    BOOST_CHECK_EQUAL(1, (context<>::apply<fake_data<int, type_list<>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(equal_context) {
    BOOST_CHECK_EQUAL(3, (context<int, double>::apply<fake_data<int, type_list<int, double>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(not_equal_different_size) {
    BOOST_CHECK_EQUAL(0, (context<int>::apply<fake_data<int, type_list<int, double>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(equal_context_not_plain_type) {
    BOOST_CHECK_EQUAL(3, (context<int, double>::apply<fake_data<int, type_list<const int&, double>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(not_equal_context_advance) {
    BOOST_CHECK_EQUAL(0, (context<int, double>::apply<fake_data<int, type_list<int, double, float>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(equal_context_advance) {
    BOOST_CHECK_EQUAL(4, (context<int, double, float>::apply<fake_data<int, type_list<char, int, double, float>, fake_scope<>>>::type::value));
}

} // namespace bindings
} // namespace di
} // namespace boost

