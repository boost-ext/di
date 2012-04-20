//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "boost/di/aux/utility.hpp"
#include "boost/di/injector.hpp"

namespace boost {
namespace di {
namespace test {
namespace ut {

struct fake_pool { };

struct module : aux::module
{
    typedef fake_pool pool;
    typedef mpl::vector<a, b, c> dependencies;

    const pool& get_pool() const { return pool_; }
    pool pool_;
};

BOOST_AUTO_TEST_CASE(injector_ctor_empty)
{
    typedef injector<> injector_t;

    injector_t injector_; //have to compile
    (void)injector;
}

BOOST_AUTO_TEST_CASE(injector_simple_module)
{
    typedef injector<module> injector_t;
    module module_;

    injector_t injector_(module_); //have to compile
    (void)injector_;
}

} // namespace ut
} // namespace test
} // namespace di
} // namespace boost

