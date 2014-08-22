//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/binder.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include <memory>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/bindings/context.hpp"
#include "boost/di/named.hpp"
#include "common/fakes/fake_scope.hpp"
#include "common/fakes/fake_dependency.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace core {

template<typename... Ts>
using context = bindings::context<Ts...>;

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
              , type_list<>
              , type_list<>
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
              , type_list<>
              , type_list<
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
              , type_list<>
              , type_list<
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
              , type_list<>
              , type_list<
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
              , type_list<>
              , type_list<
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
              , type_list<>
              , type_list<
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
              , type_list<>
              , type_list<
                    fake_dependency<fake_scope<>, if0, if0>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_basic) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
          , binder_resolve<
                int
              , type_list<a, b>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
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
              , type_list<c1>
              , type_list<
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
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
          , binder_resolve<
                int
              , type_list<a, b>
              , type_list<
                    fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_many_end) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
          , binder_resolve<
                int
              , type_list<a, b>
              , type_list<
                    fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
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
              , type_list<a>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
                >
              , fake_dependency<fake_scope<>, int>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_other_types) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
          , binder_resolve<
                int
              , type_list<a, b>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b>>>::type
                  , fake_dependency<fake_scope<>, float, float, no_name, type_list<context<a, b>>>::type
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
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b, c>>>::type
          , binder_resolve<
                int
              , type_list<a, b, c>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_empty_context) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                int
              , type_list<>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_diff_context) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
          , binder_resolve<
                int
              , type_list<c, a, b>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_short_context) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c>>>::type
          , binder_resolve<
                int
              , type_list<c>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_to_long_context) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int>::type
          , binder_resolve<
                int
              , type_list<a, b, c, d>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b>>>::type
                  , fake_dependency<fake_scope<>, int>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, a, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<b, c, c>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<a, a, a>>>::type
                  , fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

using context_priority_t = mpl::vector<
    type_list<
        fake_dependency<fake_scope<true>, int, int>::type
      , fake_dependency<fake_scope<false>, int, int>::type
    >
  , type_list<
        fake_dependency<fake_scope<false>, int, int>::type
      , fake_dependency<fake_scope<true>, int, int>::type
    >
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(context_priority, TDeps, context_priority_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<true>, int>::type
          , typename binder_resolve<
                int
              , type_list<>
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
              , type_list<>
              , type_list<
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
              , type_list<>
              , type_list<
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
              , type_list<>
              , type_list<
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
            fake_dependency<fake_scope<>, named<int, none_t>, int>::type
          , binder_resolve<
                named<aux::shared_ptr<int>, none_t>
              , type_list<>
              , type_list<>
              , fake_dependency<fake_scope<>, named<int, none_t>, int>::type
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(call_context) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c1>, context<c2>>>::type
          , binder_resolve<
                int
              , type_list<c2>
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c1>, context<c2>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

using multiple_calls_t = mpl::vector<
    type_list<c1>
  , type_list<c2, c3>
> ;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls, TContext, multiple_calls_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c1>, context<c2, c3>>>::type
          , typename binder_resolve<
                int
              , TContext
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c1>, context<c2, c3>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

using multiple_calls_many_t = mpl::vector<
    type_list<c1>
  , type_list<c1, c2, c3, c4>
  , type_list<c2, c3, c4>
  , type_list<c3, c4>
  , type_list<c5>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls_many, TContext, multiple_calls_many_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c1>, context<c5>, context<c3, c4>>>::type
          , typename binder_resolve<
                int
              , TContext
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c1>, context<c5>, context<c3, c4>>>::type
                >
              , fake_dependency<fake_scope<>, void>::type
            >::type
        >::value
    ));
}

using multiple_calls_not_found_t = mpl::vector<
    type_list<c3>
  , type_list<c6>
  , type_list<c3, c2>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls_not_found, TContext, multiple_calls_not_found_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope<>, int, int>::type
          , typename binder_resolve<
                int
              , TContext
              , type_list<
                    fake_dependency<fake_scope<>, int, int, no_name, type_list<context<c1>, context<c5>, context<c3, c4>>>::type
                >
              , fake_dependency<fake_scope<>, int, int>::type
            >::type
        >::value
    ));
}

} // namespace core
} // namespace di
} // namespace boost

