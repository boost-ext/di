//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/mpl::vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "boost/di/named.hpp"
#include "boost/di/detail/binder.hpp"

namespace boost {
namespace di {
namespace detail {
namespace test {

template<
    typename TExpected
  , typename TGiven = TExpected
  , typename TContext = mpl::vector0<>
>
struct aux::dependency
{
    typedef is_same<mpl::_1, TExpected> bind;
    typedef TContext context;
    typedef TExpected expected;
    typedef TGiven given;

    template<typename Expected, typename Given>
    struct rebind
    {
        typedef aux::dependency<Expected, Given, TContext> type;
    };
};

template<
    typename TExpected
  , typename TGiven = TExpected
  , typename TContext = mpl::vector0<>
>
struct aux::dependency_base_of
{
    typedef is_base_of<mpl::_1, TExpected> bind;
    typedef TContext context;
    typedef TExpected expected;
    typedef TGiven given;

    template<typename Expected, typename Given>
    struct rebind
    {
        typedef aux::dependency<Expected, Given, TContext> type;
    };
};

class a { };
class b { };
class c { };
class d { };
class i { };
class impl : public i { };

BOOST_AUTO_TEST_CASE(binder_empty)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int>
          , binder<
                int
              , mpl::vector0<>
              , mpl::vector0<>
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_one)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int>
          , binder<
                int
              , mpl::vector0<>
              , mpl::vector<
                    aux::dependency<int>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_found)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<float>
          , binder<
                float
              , mpl::vector0<>
              , mpl::vector<
                    aux::dependency<int>
                  , aux::dependency<float>
                  , aux::dependency<double>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_found_many)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<float>
          , binder<
                float
              , mpl::vector0<>
              , mpl::vector<
                    aux::dependency<int>
                  , aux::dependency<float>
                  , aux::dependency<float>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_not_found)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<double>
          , binder<
                double
              , mpl::vector0<>
              , mpl::vector<
                    aux::dependency<int>
                  , aux::dependency<float>
                  , aux::dependency<float>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<a> >
                   , aux::dependency<int, int, mpl::vector<a, b> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_many)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    aux::dependency<int>
                  , aux::dependency<int, int, mpl::vector<a> >
                  , aux::dependency<int, int, mpl::vector<a, b> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_contextManyEnd)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    aux::dependency<int>
                  , aux::dependency<int, int, mpl::vector<b> >
                  , aux::dependency<int, int, mpl::vector<a, b> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_not_found)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int>
          , binder<
                int
              , mpl::vector<a>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<b> >
                  , aux::dependency<int, int, mpl::vector<a, b> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_other_types)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<a> >
                  , aux::dependency<int, int, mpl::vector<a, b> >
                  , aux::dependency<float, float, mpl::vector<a, b> >
                  , aux::dependency<double>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int, int, mpl::vector<a, b, c> >
          , binder<
                int
              , mpl::vector<a, b, c>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<a, b, c> >
                  , aux::dependency<int, int, mpl::vector<a> >
                  , aux::dependency<int, int, mpl::vector<b> >
                  , aux::dependency<int>
                  , aux::dependency<int, int, mpl::vector<b, c> >
                  , aux::dependency<int, int, mpl::vector<b, a, c> >
                  , aux::dependency<int, int, mpl::vector<b, c, c> >
                  , aux::dependency<int, int, mpl::vector<a, a, a> >
                  , aux::dependency<int, int, mpl::vector<c> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_empty_call_stack)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int>
          , binder<
                int
              , mpl::vector<>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<a, b, c> >
                  , aux::dependency<int, int, mpl::vector<a> >
                  , aux::dependency<int, int, mpl::vector<b> >
                  , aux::dependency<int>
                  , aux::dependency<int, int, mpl::vector<b, c> >
                  , aux::dependency<int, int, mpl::vector<b, a, c> >,
                  , aux::dependency<int, int, mpl::vector<b, c, c> >,
                  , aux::dependency<int, int, mpl::vector<a, a, a> >,
                  , aux::dependency<int, int, mpl::vector<c> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_diff_call_stack)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int, int, mpl::vector<b> >
          , binder<
                int
              , mpl::vector<C, a, b>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<a, b, c> >
                  , aux::dependency<int, int, mpl::vector<a> >
                  , aux::dependency<int, int, mpl::vector<b> >
                  , aux::dependency<int>
                  , aux::dependency<int, int, mpl::vector<b, c> >
                  , aux::dependency<int, int, mpl::vector<b, a, c> >
                  , aux::dependency<int, int, mpl::vector<b, c, c> >
                  , aux::dependency<int, int, mpl::vector<a, a, a> >
                  , aux::dependency<int, int, mpl::vector<c> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_short_call_stack)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int, int, mpl::vector<c> >
          , binder<
                int
              , mpl::vector<c>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<a, b, c> >
                  , aux::dependency<int, int, mpl::vector<a> >
                  , aux::dependency<int, int, mpl::vector<b> >
                  , aux::dependency<int>
                  , aux::dependency<int, int, mpl::vector<b, c> >
                  , aux::dependency<int, int, mpl::vector<b, a, c> >
                  , aux::dependency<int, int, mpl::vector<b, c, c> >
                  , aux::dependency<int, int, mpl::vector<a, a, a> >
                  , aux::dependency<int, int, mpl::vector<c> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_to_long_call_stack)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int>
          , binder<
                int
              , mpl::vector<a, b, c, D>
              , mpl::vector<
                    aux::dependency<int, int, mpl::vector<a, b, c> >
                  , aux::dependency<int, int, mpl::vector<a> >
                  , aux::dependency<int, int, mpl::vector<b> >
                  , aux::dependency<int>
                  , aux::dependency<int, int, mpl::vector<b, c> >
                  , aux::dependency<int, int, mpl::vector<b, a, c> >
                  , aux::dependency<int, int, mpl::vector<b, c, c> >
                  , aux::dependency<int, int, mpl::vector<a, a, a> >
                  , aux::dependency<int, int, mpl::vector<c> >
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_base_of_fail)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<I>
          , binder<
                i,
              , mpl::vector0<>
              , mpl::vector<
                    aux::dependency_base_of<a>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_base_of_successful)
{
    BOOST_CHECK((
        is_same<
            aux::dependency_base_of<impl>
          , binder<
                i
              , mpl::vector0<>
              , mpl::vector<
                    aux::dependency_base_of<impl>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_complex_type)
{
    BOOST_CHECK((
        is_same<
            aux::dependency<int>
          , binder<
                shared_ptr<int>
              , mpl::vector0<>
              , mpl::vector<
                    aux::dependency<int>
                >
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_named_type)
{
    BOOST_CHECK((
        is_same<
            aux::dependency< named<int, mpl::_1>, int>
          , binder<
                named< shared_ptr<int>, mpl::_1>
              , mpl::vector0<>
              , mpl::vector0<>
              , aux::dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

} // namespace test
} // namespace detail
} // namespace di
} // namespace boost

