//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/convertible_copy.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>

namespace boost {
namespace di {
namespace convertibles {

struct i
{
    virtual ~i() { }
    virtual void dummy() = 0;
};

struct impl : public i
{
    virtual void dummy() { };
};

BOOST_AUTO_TEST_CASE(interface_compile) {
    std::unique_ptr<i> object = convertible_copy<i>([]{ return new impl(); });
    BOOST_CHECK(dynamic_cast<impl*>(object.get()));
}

BOOST_AUTO_TEST_CASE(ptr) {
    const int i = 42;
    std::unique_ptr<int> object = convertible_copy<int>([]{ return new int(i); });
    BOOST_CHECK_EQUAL(i, *object);
}

} // namespace convertibles
} // namespace di
} // namespace boost

