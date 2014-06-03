//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/wrappers/copy.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <boost/type.hpp>

namespace boost {
namespace di {
namespace wrappers {

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
    aux::unique_ptr<interface> object(
        copy<interface>([]{ return new implementation(); })(type<aux::unique_ptr<interface>>()));
    BOOST_CHECK(dynamic_cast<implementation*>(object.get()));
}

BOOST_AUTO_TEST_CASE(to_ptr) {
    aux::unique_ptr<int> object((copy<int>(return_i))(type<int*>()));
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    aux::shared_ptr<int> object((copy<int>(return_i))(type<aux::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_shared_ptr_other) {
    aux_::shared_ptr<int> object((copy<int>(return_i))(type<aux_::shared_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, *object);
}

BOOST_AUTO_TEST_CASE(to_auto_ptr) {
    aux::unique_ptr<aux::auto_ptr<int> > object((copy<int>(return_i))(type<aux::auto_ptr<int>*>()));
    BOOST_CHECK_EQUAL(i, **object);
}

BOOST_AUTO_TEST_CASE(to_unique_ptr) {
    aux::unique_ptr<int> object((copy<int>(return_i))(type<aux::unique_ptr<int>>()));
    BOOST_CHECK_EQUAL(i, *object);
}

} // namespace wrappers
} // namespace di
} // namespace boost

