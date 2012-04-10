//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include "QDeps/Back/Aux/Dependency.hpp"
#include "QDeps/Back/Detail/Creator.hpp"
#include "QDeps/Back/Scopes/PerRequest.hpp"
#include "QDeps/Back/Scopes/Singleton.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{
namespace UT
{

using namespace boost;
using namespace boost::mpl;

class FakePool { };

template<typename T, T Value>
struct Dependency
{
    typedef T Given;
    typedef T Expected;
    typedef vector0<> Context;
    typedef void Scope;
    typedef is_same<_1, T> Bind;
    typedef vector0<> Ctor;

    template<typename TPool> struct ResultType
    {
        typedef T type;
    };

    T create(FakePool&) { return Value; }
};

template<typename T> struct Entries : T { };

BOOST_AUTO_TEST_CASE(Basic)
{
    const int i = 42;
    typedef Dependency<int, i> Dep;
    FakePool pool;
    Entries<Dep> entries;

    EXPECT_EQ(i, (Creator
        <
            vector
            <
                Dependency<int, i>
            >,
            FakePool
        >::execute<int, vector0<> >(entries, pool)
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

