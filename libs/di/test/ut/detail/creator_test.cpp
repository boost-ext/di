//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include "di/back/detail/creator.hpp"

namespace di
{
namespace back
{
namespace detail
{
namespace ut
{

using namespace boost;
using namespace mpl;

class Fakepool { };

template<typename T, T value>
struct dependency
{
    typedef T given;
    typedef T expected;
    typedef vector0<> context;
    typedef void scope;
    typedef is_same<_1, T> bind;
    typedef vector0<> ctor;

    template<typename TPool> struct result_type
    {
        typedef T type;
    };

    T create(const Fakepool&) { return value; }
};

template<typename T> struct Entries : T { };

BOOST_AUTO_TEST_CASE(CreateSimple)
{
    const int i = 42;
    typedef dependency<int, i> Dep;
    Fakepool pool;
    Entries<Dep> entries;

    BOOST_CHECK_EQUAL(i, (creator
        <
            vector<Dep>,
            Fakepool
        >::execute<int, vector0<> >(entries, pool)
    ));
}

} // namespace ut
} // namespace detail
} // namespace back
} // namespace di

