//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/binder.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <memory>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"
#include "common/fakes/fake_scope.hpp"
#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_call_stack.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace detail {

template<
   typename T
 , typename TCallStack
 , typename TDependecies
 , typename TDefault
>
struct binder_resolve
    : binder<TDependecies>::template resolve<T, TCallStack, TDefault>
{ };

BOOST_AUTO_TEST_CASE(empty) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                int
              , mpl::vector0<>
              , mpl::vector0<>
              , fake_dependency<fake_scope<>, int>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                int
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int>::type
                >
              , fake_dependency<fake_scope<>, int>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(found) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, float>::type
          , binder_resolve<
                float
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, float>::type
                  , fake_dependency<fake_scope<>, double>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(found_many) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, float>::type
          , binder_resolve<
                float
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, float>::type
                  , fake_dependency<fake_scope<>, float>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(not_found) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, double>::type
          , binder_resolve<
                double
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, float>::type
                  , fake_dependency<fake_scope<>, float>::type
                >
              , fake_dependency<fake_scope<>, double>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(if_base) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, if0, c0if0>::type
          , binder_resolve<
                if0
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, if0, c0if0>
                >
              , fake_dependency<fake_scope<>, void>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(if_external) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, if0, if0>::type
          , binder_resolve<
                if0
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, if0, if0>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
          , binder_resolve<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
                >
              , fake_dependency<fake_scope<>, no_name>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_if) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, if0, c0if0>::type
          , binder_resolve<
                if0
              , mpl::vector<c1>
              , mpl::vector<
                    fake_dependency<fake_scope<>, if0, c0if0>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_many) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
          , binder_resolve<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_many_end) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
          , binder_resolve<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_not_found) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                int
              , mpl::vector<a>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
                >
              , fake_dependency<fake_scope<>, int>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_other_types) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
          , binder_resolve<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b>>>::type
                  , fake_dependency<fake_scope<>, float, float, no_name, mpl::vector<fake_call_stack<a, b>>>::type
                  , fake_dependency<fake_scope<>, double>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b, c>>>::type
          , binder_resolve<
                int
              , mpl::vector<a, b, c>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_empty_fake_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                int
              , mpl::vector<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_diff_fake_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
          , binder_resolve<
                int
              , mpl::vector<c, a, b>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_short_fake_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c>>>::type
          , binder_resolve<
                int
              , mpl::vector<c>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_to_long_fake_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                int
              , mpl::vector<a, b, c, d>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

using context_priority_t = mpl::vector<
    mpl::vector<
        fake_dependency<fake_scope<1>, int, int>::type
      , fake_dependency<fake_scope<0>, int, int>::type
      , fake_dependency<fake_scope<3>, int, int>::type
    >
  , mpl::vector<
        fake_dependency<fake_scope<0>, int, int>::type
      , fake_dependency<fake_scope<1>, int, int>::type
      , fake_dependency<fake_scope<3>, int, int>::type
    >
  , mpl::vector<
        fake_dependency<fake_scope<3>, int, int>::type
      , fake_dependency<fake_scope<1>, int, int>::type
      , fake_dependency<fake_scope<0>, int, int>::type
    >
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(context_priority, TDeps, context_priority_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<3>, int>::type
          , typename binder_resolve<
                int
              , mpl::vector<>
              , TDeps
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(base_of_fail) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, i>::type
          , binder_resolve<
                i
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, a>::type
                >
              , fake_dependency<fake_scope<>, i>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(base_of_successful) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, impl>::type
          , binder_resolve<
                i
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, impl>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(complex_type) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                aux::shared_ptr<int>
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_int_type) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, named<int, mpl::_1>, int>::type
          , binder_resolve<
                named<aux::shared_ptr<int>, mpl::_1>
              , mpl::vector0<>
              , mpl::vector0<>
              , fake_dependency<fake_scope<>, named<int, mpl::_1>, int>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(call_fake_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c1>, fake_call_stack<c2>>>::type
          , binder_resolve<
                int
              , mpl::vector<c2>
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c1>, fake_call_stack<c2>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c1>
  , mpl::vector<c2, c3>
> multiple_calls_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls, TContext, multiple_calls_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c1>, fake_call_stack<c2, c3>>>::type
          , typename binder_resolve<
                int
              , TContext
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c1>, fake_call_stack<c2, c3>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c1>
  , mpl::vector<c1, c2, c3, c4>
  , mpl::vector<c2, c3, c4>
  , mpl::vector<c3, c4>
  , mpl::vector<c5>
> multiple_calls_many_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls_many, TContext, multiple_calls_many_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c1>, fake_call_stack<c5>, fake_call_stack<c3, c4>>>::type
          , typename binder_resolve<
                int
              , TContext
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c1>, fake_call_stack<c5>, fake_call_stack<c3, c4>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c3>
  , mpl::vector<c6>
  , mpl::vector<c3, c2>
> multiple_calls_not_found_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls_not_found, TContext, multiple_calls_not_found_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int>::type
          , typename binder_resolve<
                int
              , TContext
              , mpl::vector<
                    fake_dependency<fake_scope<>, int, int, no_name, mpl::vector<fake_call_stack<c1>, fake_call_stack<c5>, fake_call_stack<c3, c4>>>::type
                >
              , fake_dependency<fake_scope<>, int, int>::type
            >::type
        >::value
    ));
}

} // namespace detail
} // namespace di
} // namespace boost

