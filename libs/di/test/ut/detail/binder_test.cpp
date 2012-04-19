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
#include "di/back/detail/binder.hpp"
#include "di/utility/named.hpp"

namespace di
{
namespace back
{
namespace detail
{
namespace ut
{


template<typename TExpected, typename TGiven = TExpected, typename TContext = vector0<> > struct dependency
{
    typedef is_same<_1, TExpected> bind;
    typedef TContext context;
    typedef TExpected expected;
    typedef TGiven given;

    template<typename expected, typename given> struct rebind
    {
        typedef dependency<expected, given, TContext> type;
    };
};

template<typename TExpected, typename TGiven = TExpected, typename TContext = vector0<> > struct dependencybaseOf
{
    typedef is_base_of<_1, TExpected> bind;
    typedef TContext context;
    typedef TExpected expected;
    typedef TGiven given;

    template<typename expected, typename given> struct rebind
    {
        typedef dependency<expected, given, TContext> type;
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
            dependency<int>,
            binder
            <
                int,
                vector0<>,
                vector0<>,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(One)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int>,
            binder
            <
                int,
                vector0<>,
                vector
                <
                    dependency<int>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(Found)
{
    BOOST_CHECK((
        is_same
        <
            dependency<float>,
            binder
            <
                float,
                vector0<>,
                vector
                <
                    dependency<int>,
                    dependency<float>,
                    dependency<double>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(FoundMany)
{
    BOOST_CHECK((
        is_same
        <
            dependency<float>,
            binder
            <
                float,
                vector0<>,
                vector
                <
                    dependency<int>,
                    dependency<float>,
                    dependency<float>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(NotFound)
{
    BOOST_CHECK((
        is_same
        <
            dependency<double>,
            binder
            <
                double,
                vector0<>,
                vector
                <
                    dependency<int>,
                    dependency<float>,
                    dependency<float>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int, int, vector<A, B> >,
            binder
            <
                int,
                vector<A, B>,
                vector
                <
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<A, B> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextMany)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int, int, vector<A, B> >,
            binder
            <
                int,
                vector<A, B>,
                vector
                <
                    dependency<int>,
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<A, B> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextManyEnd)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int, int, vector<A, B> >,
            binder
            <
                int,
                vector<A, B>,
                vector
                <
                    dependency<int>,
                    dependency<int, int, vector<B> >,
                    dependency<int, int, vector<A, B> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextNotFound)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int>,
            binder
            <
                int,
                vector<A>,
                vector
                <
                    dependency<int, int, vector<B> >,
                    dependency<int, int, vector<A, B> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextOtherTypes)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int, int, vector<A, B> >,
            binder
            <
                int,
                vector<A, B>,
                vector
                <
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<A, B> >,
                    dependency<float, float, vector<A, B> >,
                    dependency<double>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextLongwithOrder)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int, int, vector<A, B, C> >,
            binder
            <
                int,
                vector<A, B, C>,
                vector
                <
                    dependency<int, int, vector<A, B, C> >,
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<B> >,
                    dependency<int>,
                    dependency<int, int, vector<B, C> >,
                    dependency<int, int, vector<B, A, C> >,
                    dependency<int, int, vector<B, C, C> >,
                    dependency<int, int, vector<A, A, A> >,
                    dependency<int, int, vector<C> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextLongwithOrderempty_call_stack)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int>,
            binder
            <
                int,
                vector<>,
                vector
                <
                    dependency<int, int, vector<A, B, C> >,
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<B> >,
                    dependency<int>,
                    dependency<int, int, vector<B, C> >,
                    dependency<int, int, vector<B, A, C> >,
                    dependency<int, int, vector<B, C, C> >,
                    dependency<int, int, vector<A, A, A> >,
                    dependency<int, int, vector<C> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextLongwithOrderDiffCallStack)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int, int, vector<B> >,
            binder
            <
                int,
                vector<C, A, B>,
                vector
                <
                    dependency<int, int, vector<A, B, C> >,
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<B> >,
                    dependency<int>,
                    dependency<int, int, vector<B, C> >,
                    dependency<int, int, vector<B, A, C> >,
                    dependency<int, int, vector<B, C, C> >,
                    dependency<int, int, vector<A, A, A> >,
                    dependency<int, int, vector<C> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextLongwithOrderShortCallStack)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int, int, vector<C> >,
            binder
            <
                int,
                vector<C>,
                vector
                <
                    dependency<int, int, vector<A, B, C> >,
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<B> >,
                    dependency<int>,
                    dependency<int, int, vector<B, C> >,
                    dependency<int, int, vector<B, A, C> >,
                    dependency<int, int, vector<B, C, C> >,
                    dependency<int, int, vector<A, A, A> >,
                    dependency<int, int, vector<C> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(contextLongwithOrderToLongCallStack)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int>,
            binder
            <
                int,
                vector<A, B, C, D>,
                vector
                <
                    dependency<int, int, vector<A, B, C> >,
                    dependency<int, int, vector<A> >,
                    dependency<int, int, vector<B> >,
                    dependency<int>,
                    dependency<int, int, vector<B, C> >,
                    dependency<int, int, vector<B, A, C> >,
                    dependency<int, int, vector<B, C, C> >,
                    dependency<int, int, vector<A, A, A> >,
                    dependency<int, int, vector<C> >
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(baseOfFail)
{
    BOOST_CHECK((
        is_same
        <
            dependency<I>,
            binder
            <
                I,
                vector0<>,
                vector
                <
                    dependencybaseOf<A>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(baseOfSuccessful)
{
    BOOST_CHECK((
        is_same
        <
            dependencybaseOf<Impl>,
            binder
            <
                I,
                vector0<>,
                vector
                <
                    dependencybaseOf<Impl>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(ComplexType)
{
    BOOST_CHECK((
        is_same
        <
            dependency<int>,
            binder
            <
                shared_ptr<int>,
                vector0<>,
                vector
                <
                    dependency<int>
                >,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(namedType)
{
    BOOST_CHECK((
        is_same
        <
            dependency< named<int, _1>, int>,
            binder
            <
                utility::named< shared_ptr<int>, _1>,
                vector0<>,
                vector0<>,
                dependency<_1, _2>
            >::type
        >::value
    ));
}

} // namespace ut
} // namespace detail
} // namespace back
} // namespace di

