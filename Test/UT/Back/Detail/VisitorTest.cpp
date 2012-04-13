//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include "QDeps/Back/Detail/Visitor.hpp"

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

template<typename T>
struct Dependency
{
    typedef T Given;
    typedef T Expected;
    typedef vector0<> Context;
    typedef void Scope;
    typedef is_same<_1, T> Bind;
    typedef vector0<> Ctor;
};

template<typename T>
struct VisitorMock
{
    template<typename TDependency> void operator()() const
    {
        BOOST_CHECK_EQUAL(typeid(T).name(), typeid(typename TDependency::Given).name());
    }
};

BOOST_AUTO_TEST_CASE(VisitSimple)
{
    typedef Dependency<int> Dep;
    VisitorMock<int> visitorMock;

    Visitor< vector<Dep> >::execute<int, vector0<> >(visitorMock);
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

