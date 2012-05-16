//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "boost/di/injector.hpp"
#include "data.hpp"

namespace boost {
namespace di {

struct fake_pool { };

struct module
{
    typedef mpl::vector<a, b, c> deps;
    typedef fake_pool pool;
    pool pool_;
};

BOOST_AUTO_TEST_CASE(ctor_empty)
{
    typedef injector<> injector_t;

    injector_t injector_; //have to compile
    (void)injector_;
}

BOOST_AUTO_TEST_CASE(simple_module)
{
    typedef injector<module> injector_t;
    module module_;

    injector_t injector_(module_); //have to compile
    (void)injector_;
}

BOOST_AUTO_TEST_CASE(install)
{
}

BOOST_AUTO_TEST_CASE(pre_and_install)
{
}

} // namespace di
} // namespace boost

