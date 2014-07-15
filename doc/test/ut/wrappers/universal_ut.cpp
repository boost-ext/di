//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/wrappers/universal.hpp"

#include <boost/test/unit_test.hpp>
#include <vector>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"
#include "common/fakes/fake_wrapper.hpp"

namespace boost {
namespace di {
namespace wrappers {

BOOST_AUTO_TEST_CASE(convert_to_value_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<int> c(refs, fake_wrapper_<int>(i));
    BOOST_CHECK_EQUAL(i, static_cast<int>(c));
}

BOOST_AUTO_TEST_CASE(convert_to_named_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<named<int>> c(refs, fake_wrapper_<int>(i));
    BOOST_CHECK_EQUAL(i, c.operator int());
    BOOST_CHECK_EQUAL(i, c.operator named<int>());
}

BOOST_AUTO_TEST_CASE(convert_to_named_const_ref_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<named<const int&>> c(refs, fake_wrapper_<const int&>(i));
    BOOST_CHECK_EQUAL(i, c.operator named<const int&>());
}

BOOST_AUTO_TEST_CASE(convert_to_const_named_ref_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<const named<int>&> c(refs, fake_wrapper_<int>(i));
    BOOST_CHECK_EQUAL(i, c.operator const named<int>&());
}

BOOST_AUTO_TEST_CASE(convert_to_const_named_const_ref_ref_type) {
    named<const int&> i(42);
    std::vector<aux::shared_ptr<void>> refs;
    universal<const named<const int&>&> c(refs, fake_wrapper_<named<const int&>>(i));
    BOOST_CHECK_EQUAL(i, c.operator const named<const int&>&());
}

BOOST_AUTO_TEST_CASE(convert_to_ref_type) {
    int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<int&> c(refs, fake_wrapper_<int&>(i));
    BOOST_CHECK_EQUAL(i, static_cast<int&>(c));
}

BOOST_AUTO_TEST_CASE(convert_to_const_ref_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<const int&> c(refs, fake_wrapper_<const int&>(i));
    BOOST_CHECK_EQUAL(i, static_cast<const int&>(c));
}

BOOST_AUTO_TEST_CASE(convert_to_const_ref_type_is_convertible) {
    to_ref to_ref_;
    std::vector<aux::shared_ptr<void>> refs;
    universal<const to_ref&> c(refs, fake_wrapper_<const to_ref&>(to_ref_));
    BOOST_CHECK(&to_ref_ == &static_cast<const to_ref&>(c));
}

BOOST_AUTO_TEST_CASE(convert_to_const_ref_type_is_not_convertible) {
    not_to_ref not_to_ref_;
    std::vector<aux::shared_ptr<void>> refs;
    universal<const not_to_ref&> c(refs, fake_wrapper_<const not_to_ref&>(not_to_ref_));
    BOOST_CHECK(&not_to_ref_ != &static_cast<const not_to_ref&>(c));
}

BOOST_AUTO_TEST_CASE(convert_to_auto_ptr_type) {
    const int i = 42;
    std::vector<aux::shared_ptr<void>> refs;
    universal<std::auto_ptr<int>> c(refs, fake_wrapper_<std::auto_ptr<int>>(new int(i)));
    BOOST_CHECK_EQUAL(i, *static_cast<std::auto_ptr<int>>(c));
}

} // namespace wrappers
} // namespace di
} // namespace boost

