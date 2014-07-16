//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/any_type.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

#include "common/fakes/fake_creator.hpp"
#include "common/fakes/fake_allocator.hpp"

#include "boost/di/aux_/memory.hpp"

namespace boost {
namespace di {
namespace core {

BOOST_AUTO_TEST_CASE(default_ctor) {
    any_type<> any_type_; // has to compile
}

BOOST_AUTO_TEST_CASE(conversion_to_const_ref) {
    using type = const int&;
    fake_creator<type> creator;
    fake_allocator allocator;
    none_t deps;
    none_t refs;

    BOOST_CHECK_EQUAL(
        0
      , type(any_type<void, mpl::vector0<>, fake_creator<type>, fake_allocator>(
            creator, allocator, deps, refs, none_t(), none_t())
        )
    );
}

BOOST_AUTO_TEST_CASE(conversion_to_ref) {
    using type = int&;
    fake_creator<type> creator;
    fake_allocator allocator;
    none_t deps;
    none_t refs;
    type i = any_type<void, mpl::vector0<>, fake_creator<type>, fake_allocator>(
        creator, allocator, deps, refs, none_t(), none_t()
    );

    BOOST_CHECK_EQUAL(0, i);
}

BOOST_AUTO_TEST_CASE(conversion_to_copy) {
    using type = int;
    fake_creator<type> creator;
    fake_allocator allocator;
    none_t deps;
    none_t refs;

    BOOST_CHECK_EQUAL(
        0
      , type(any_type<void, mpl::vector0<>, fake_creator<type>, fake_allocator>(
            creator, allocator, deps, refs, none_t(), none_t())
        )
    );
}

} // namespace core
} // namespace di
} // namespace boost

