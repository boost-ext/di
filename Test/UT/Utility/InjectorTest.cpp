//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "QDeps/Back/Module.hpp"
#include "QDeps/Utility/Injector.hpp"

namespace QDeps
{
namespace Utility
{
namespace UT
{

using namespace boost::mpl;

class A { };
class B { };
class C { };
class E { };

struct Module : Back::Module
{
    struct Dependencies : boost::mpl::vector<A, B, C> { };
    struct Externals : boost::mpl::vector<E> { };
};

BOOST_AUTO_TEST_CASE(CtorEmpty)
{
    typedef Injector<> Inj;

    Inj l_inj; //have to compile
    (void)l_inj;

    BOOST_CHECK((
        equal
        <
            vector0<>,
            Inj::Dependencies
        >::value
    ));

    BOOST_CHECK((
        equal
        <
            vector0<>,
            Inj::Externals
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(SimpleModule)
{
    typedef Injector<Module> Inj;

    Inj l_inj; //have to compile
    (void)l_inj;

    BOOST_CHECK((
        equal
        <
            vector<C, B, A>,
            Inj::Dependencies
        >::value
    ));

    BOOST_CHECK((
        equal
        <
            vector<E>,
            Inj::Externals
        >::value
    ));
}

} // namespace UT
} // namespace Utility
} // namespace QDeps

