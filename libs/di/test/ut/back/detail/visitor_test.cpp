//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include "di/back/detail/visitor.hpp"

namespace di
{
namespace back
{
namespace detail
{
namespace ut
{

using namespace boost;
using namespace boost::mpl;

template<typename T>
struct dependency
{
    typedef T given;
    typedef T expected;
    typedef vector0<> context;
    typedef void scope;
    typedef is_same<_1, T> bind;
    typedef vector0<> ctor;
};

template<typename T>
struct visitorMock
{
    template<typename TDependency> void operator()() const
    {
        BOOST_CHECK_EQUAL(typeid(T).name(), typeid(typename TDependency::given).name());
    }
};

BOOST_AUTO_TEST_CASE(VisitSimple)
{
    typedef dependency<int> Dep;
    visitorMock<int> visitorMock;

    visitor< vector<Dep> >::execute<int, vector0<> >(visitorMock);
}

} // namespace ut
} // namespace detail
} // namespace back
} // namespace di

