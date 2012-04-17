//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "di/back/module.hpp"
#include "di/utility/injector.hpp"

namespace di
{
namespace utility
{
namespace ut
{

using namespace boost::mpl;

class A { };
class B { };
class C { };

struct fake_pool { };

struct module : back::module
{
    typedef fake_pool pool;
    typedef boost::mpl::vector<A, B, C> dependencies;

    const pool& get_pool() const { return pool_; }
    pool pool_;
};

BOOST_AUTO_TEST_CASE(ctorEmpty)
{
    typedef injector<> Inj;

    Inj l_inj; //have to compile
    (void)l_inj;
}

BOOST_AUTO_TEST_CASE(Simplemodule)
{
    typedef injector<module> Inj;
    module module;

    Inj l_inj(module); //have to compile
    (void)l_inj;
}

} // namespace ut
} // namespace utility
} // namespace di

