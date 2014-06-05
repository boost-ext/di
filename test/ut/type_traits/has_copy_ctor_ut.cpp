//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/has_copy_ctor.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/noncopyable.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct trivial_ctor { };
struct default_ctor { default_ctor(int = 0) { } };
struct copy_ctor { copy_ctor(const copy_ctor&); };
class private_copy_ctor { private_copy_ctor(const private_copy_ctor&); };
class non_const_copy_ctor { non_const_copy_ctor(non_const_copy_ctor&) { } };
class noncopyable_ctor : boost::noncopyable { };

BOOST_AUTO_TEST_CASE(copy_ctors) {
    BOOST_CHECK(has_copy_ctor<int>::value);
    BOOST_CHECK(has_copy_ctor<trivial_ctor>::value);
    BOOST_CHECK(has_copy_ctor<copy_ctor>::value);

    BOOST_CHECK(!has_copy_ctor<private_copy_ctor>::value);
    BOOST_CHECK(!has_copy_ctor<non_const_copy_ctor>::value);
}

} // namespace type_traits
} // namespace di
} // namespace boost

