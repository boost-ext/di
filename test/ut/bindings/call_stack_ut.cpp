//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/bindings/call_stack.hpp"

#include <boost/test/unit_test.hpp>

#include "common/fakes/fake_data.hpp"
#include "common/fakes/fake_scope.hpp"

namespace boost {
namespace di {
namespace bindings {

BOOST_AUTO_TEST_CASE(empty_call_stack) {
    BOOST_CHECK_EQUAL(1, (call_stack<>::apply<fake_data<int, type_list<>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(equal_call_stack) {
    BOOST_CHECK_EQUAL(3, (call_stack<int, double>::apply<fake_data<int, type_list<int, double>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(not_equal_different_size) {
    BOOST_CHECK_EQUAL(0, (call_stack<int>::apply<fake_data<int, type_list<int, double>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(equal_call_stack_not_plain_type) {
    BOOST_CHECK_EQUAL(3, (call_stack<int, double>::apply<fake_data<int, type_list<const int&, double>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(not_equal_call_stack_advance) {
    BOOST_CHECK_EQUAL(0, (call_stack<int, double>::apply<fake_data<int, type_list<int, double, float>, fake_scope<>>>::type::value));
}

BOOST_AUTO_TEST_CASE(equal_call_stack_advance) {
    BOOST_CHECK_EQUAL(4, (call_stack<int, double, float>::apply<fake_data<int, type_list<char, int, double, float>, fake_scope<>>>::type::value));
}

} // namespace bindings
} // namespace di
} // namespace boost

