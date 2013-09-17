//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/create_traits.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/config.hpp"
#include "boost/di/ctor.hpp"

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

BOOST_AUTO_TEST_CASE(create) {
    shared_ptr<empty> empty_(create_traits<empty, empty>());
    BOOST_CHECK(empty_.get());

    shared_ptr<ctor> ctor_(create_traits<ctor, ctor, int, double>(42, 42.0));
    BOOST_CHECK(ctor_.get());

    shared_ptr<int> factory_(create_traits<int, factory>());
    BOOST_CHECK(!factory_.get());

    shared_ptr<int> factory_ext_(create_traits<int, factory_ext, int>(42));
    BOOST_CHECK(factory_ext_.get());
    BOOST_CHECK_EQUAL(42, *factory_ext_);
}

} // namespace type_traits
} // namespace di
} // namespace boost

