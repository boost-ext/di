//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "QDeps/Back/Detail/Binder.hpp"
#include "QDeps/Utility/Named.hpp"

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
using namespace Utility;
using namespace Aux;

template<typename TExpected, typename TGiven = TExpected, typename TContext = vector0<> > struct Dependency
{
    typedef is_same<_1, TExpected> Bind;
    typedef TContext Context;
    typedef TExpected Expected;
    typedef TGiven Given;

    template<typename Expected, typename Given> struct Rebind
    {
        typedef Dependency<Expected, Given, TContext> type;
    };
};

template<typename TExpected, typename TGiven = TExpected, typename TContext = vector0<> > struct DependencyBaseOf
{
    typedef is_base_of<_1, TExpected> Bind;
    typedef TContext Context;
    typedef TExpected Expected;
    typedef TGiven Given;

    template<typename Expected, typename Given> struct Rebind
    {
        typedef Dependency<Expected, Given, TContext> type;
    };
};

class A { };
class B { };
class C { };
class D { };
class I { };
class Impl : public I { };

BOOST_AUTO_TEST_CASE(Empty)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int>,
            Binder
            <
                int,
                vector0<>,
                vector0<>,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(One)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int>,
            Binder
            <
                int,
                vector0<>,
                vector
                <
                    Dependency<int>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(Found)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<float>,
            Binder
            <
                float,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<double>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(FoundMany)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<float>,
            Binder
            <
                float,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<float>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(NotFound)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<double>,
            Binder
            <
                double,
                vector0<>,
                vector
                <
                    Dependency<int>,
                    Dependency<float>,
                    Dependency<float>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(Context)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int, int, vector<A, B> >,
            Binder
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<A, B> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextMany)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int, int, vector<A, B> >,
            Binder
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int>,
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<A, B> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextManyEnd)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int, int, vector<A, B> >,
            Binder
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int>,
                    Dependency<int, int, vector<B> >,
                    Dependency<int, int, vector<A, B> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextNotFound)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int>,
            Binder
            <
                int,
                vector<A>,
                vector
                <
                    Dependency<int, int, vector<B> >,
                    Dependency<int, int, vector<A, B> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextOtherTypes)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int, int, vector<A, B> >,
            Binder
            <
                int,
                vector<A, B>,
                vector
                <
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<A, B> >,
                    Dependency<float, float, vector<A, B> >,
                    Dependency<double>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextLongWithOrder)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int, int, vector<A, B, C> >,
            Binder
            <
                int,
                vector<A, B, C>,
                vector
                <
                    Dependency<int, int, vector<A, B, C> >,
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, int, vector<B, C> >,
                    Dependency<int, int, vector<B, A, C> >,
                    Dependency<int, int, vector<B, C, C> >,
                    Dependency<int, int, vector<A, A, A> >,
                    Dependency<int, int, vector<C> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextLongWithOrderEmptyCallStack)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int>,
            Binder
            <
                int,
                vector<>,
                vector
                <
                    Dependency<int, int, vector<A, B, C> >,
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, int, vector<B, C> >,
                    Dependency<int, int, vector<B, A, C> >,
                    Dependency<int, int, vector<B, C, C> >,
                    Dependency<int, int, vector<A, A, A> >,
                    Dependency<int, int, vector<C> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextLongWithOrderDiffCallStack)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int, int, vector<B> >,
            Binder
            <
                int,
                vector<C, A, B>,
                vector
                <
                    Dependency<int, int, vector<A, B, C> >,
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, int, vector<B, C> >,
                    Dependency<int, int, vector<B, A, C> >,
                    Dependency<int, int, vector<B, C, C> >,
                    Dependency<int, int, vector<A, A, A> >,
                    Dependency<int, int, vector<C> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextLongWithOrderShortCallStack)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int, int, vector<C> >,
            Binder
            <
                int,
                vector<C>,
                vector
                <
                    Dependency<int, int, vector<A, B, C> >,
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, int, vector<B, C> >,
                    Dependency<int, int, vector<B, A, C> >,
                    Dependency<int, int, vector<B, C, C> >,
                    Dependency<int, int, vector<A, A, A> >,
                    Dependency<int, int, vector<C> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ContextLongWithOrderToLongCallStack)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int>,
            Binder
            <
                int,
                vector<A, B, C, D>,
                vector
                <
                    Dependency<int, int, vector<A, B, C> >,
                    Dependency<int, int, vector<A> >,
                    Dependency<int, int, vector<B> >,
                    Dependency<int>,
                    Dependency<int, int, vector<B, C> >,
                    Dependency<int, int, vector<B, A, C> >,
                    Dependency<int, int, vector<B, C, C> >,
                    Dependency<int, int, vector<A, A, A> >,
                    Dependency<int, int, vector<C> >
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(BaseOfFail)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<I>,
            Binder
            <
                I,
                vector0<>,
                vector
                <
                    DependencyBaseOf<A>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(BaseOfSuccessful)
{
    BOOST_CHECK((
        is_same
        <
            DependencyBaseOf<Impl>,
            Binder
            <
                I,
                vector0<>,
                vector
                <
                    DependencyBaseOf<Impl>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ComplexType)
{
    BOOST_CHECK((
        is_same
        <
            Dependency<int>,
            Binder
            <
                shared_ptr<int>,
                vector0<>,
                vector
                <
                    Dependency<int>
                >,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(NamedType)
{
    BOOST_CHECK((
        is_same
        <
            Dependency< Named<int, _1>, int>,
            Binder
            <
                Utility::Named< shared_ptr<int>, _1>,
                vector0<>,
                vector0<>,
                Dependency<_1, _2>
            >::type
        >::value
    ));
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

