//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/bindings.hpp"
#include <boost/mpl/int.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/pair.hpp>

#include "boost/di/cpp_0x/scopes/deduce.hpp"

#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_scope.hpp"
#include "common/contains_all.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(bind_impl) {
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind<i, impl>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, i, impl>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, int, mpl::int_<i>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_named) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::named<void>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, int, mpl::int_<i>, void>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_when) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::when<call_stack<c0>>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, int, mpl::int_<i>, no_name, mpl::vector<call_stack<c0>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_named_when) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::named<void>::when<call_stack<c0>>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, int, mpl::int_<i>, void, mpl::vector<call_stack<c0>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_when_named) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::when<call_stack<c0>>::named<void>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, int, mpl::int_<i>, void, mpl::vector<call_stack<c0>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_bool_value) {
    const bool b = true;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_bool<b>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, bool, mpl::bool_<b>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_bool_value_named) {
    const bool b = true;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_bool<b>::named<void>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, bool, mpl::bool_<b>, void>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_bool_value_when) {
    const bool b = true;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_bool<b>::when<call_stack<c0>>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, bool, mpl::bool_<b>, no_name, mpl::vector<call_stack<c0>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_bool_value_named_when) {
    const bool b = true;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_bool<b>::named<void>::when<call_stack<c0>>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, bool, mpl::bool_<b>, void, mpl::vector<call_stack<c0>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_bool_value_when_named) {
    const bool b = true;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_bool<b>::when<call_stack<c0>>::named<void>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, bool, mpl::bool_<b>, void, mpl::vector<call_stack<c0>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, std::string, s>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_named) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::named<void>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, std::string, s, void>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_named_when) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::named<void>::when<call_stack<c0>, call_stack<c1>>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, std::string, s, void, mpl::vector<call_stack<c0>, call_stack<c1>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_when_named) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::when<call_stack<c0>, call_stack<c1>>::named<void>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, std::string, s, void, mpl::vector<call_stack<c0>, call_stack<c1>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_when) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::when<call_stack<c0>>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, std::string, s, no_name, mpl::vector<call_stack<c0>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_any) {
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind<cif0if1>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, cif0if1, cif0if1>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_any_of) {
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind<any_of<if0, if1>, cif0if1>::type
            >
          , mpl::vector<
                fake_dependency<
                    fake_scope<>
                  , cif0if1
                  , cif0if1
                  , no_name
                  , mpl::vector0<>
                  , bindings::detail::requires_<
                        bindings::type_traits::is_required_priority
                      , bindings::type_traits::is_required_type<any_of<if0, if1>>
                    >
                >::type
            >
        >::value
    ));
}

using scope_empty_types = mpl::vector<
    deduce<>
  , unique<>
  , shared<>
  , session<>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_empty, TScope, scope_empty_types) {
    BOOST_CHECK((
        contains_all<
            typename TScope::type
          , mpl::vector<>
        >::value
    ));
}

using scope_one_types = mpl::vector<
    mpl::pair<scopes::deduce, deduce<int>>
  , mpl::pair<scopes::unique<>, unique<int>>
  , mpl::pair<scopes::shared<>, shared<int>>
  , mpl::pair<scopes::session<>, session<int>>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_one, T, scope_one_types) {
    BOOST_CHECK((
        contains_all<
            typename mpl::second<T>::type
          , mpl::vector<
                typename fake_dependency<typename mpl::first<T>::type, int>::type
            >
        >::value
    ));
}

using scope_many_types = mpl::vector<
    mpl::pair<scopes::deduce, deduce<int, double, float>>
  , mpl::pair<scopes::unique<>, unique<int, double, float>>
  , mpl::pair<scopes::shared<>, shared<int, double, float>>
  , mpl::pair<scopes::session<>, session<int, double, float>>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_many, T, scope_many_types) {
    BOOST_CHECK((
        contains_all<
            typename mpl::second<T>::type
          , mpl::vector<
                typename fake_dependency<typename mpl::first<T>::type, int>::type
              , typename fake_dependency<typename mpl::first<T>::type, double>::type
              , typename fake_dependency<typename mpl::first<T>::type, float>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_with_call_stack) {
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind<i, impl>::when<call_stack<c0, c1>>::type
            >
          , mpl::vector<
                fake_dependency<fake_scope<>, i, impl, no_name, mpl::vector<call_stack<c0, c1>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_deduce_with_deduced_interface) {
    typedef fake_dependency<scopes::deduce, impl, impl>::type expected;

    BOOST_CHECK((
        contains_all<
            mpl::vector<
                bind<impl>::type
              , bind<impl, impl>::type
            >
          , mpl::vector<expected, expected>
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            deduce<impl>::type
          , mpl::vector<expected>
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            deduce<bind<impl>>::type
          , mpl::vector<expected>
        >::value
    ));
}

} // namespace di
} // namespace boost

