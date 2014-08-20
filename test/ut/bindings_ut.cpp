//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/bindings.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/string.hpp>

#include "boost/di/scopes/deduce.hpp"

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
          , type_list<
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
          , type_list<
                fake_dependency<fake_scope<>, int, int_<i>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, int, int_<i>, void>::type
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
          , type_list<
                fake_dependency<fake_scope<>, int, int_<i>, no_name, type_list<call_stack<c0>>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, int, int_<i>, void, type_list<call_stack<c0>>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, int, int_<i>, void, type_list<call_stack<c0>>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, bool, bool_<b>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, bool, bool_<b>, void>::type
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
          , type_list<
                fake_dependency<fake_scope<>, bool, bool_<b>, no_name, type_list<call_stack<c0>>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, bool, bool_<b>, void, type_list<call_stack<c0>>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, bool, bool_<b>, void, type_list<call_stack<c0>>>::type
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
          , type_list<
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
          , type_list<
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
          , type_list<
                fake_dependency<fake_scope<>, std::string, s, void, type_list<call_stack<c0>, call_stack<c1>>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, std::string, s, void, type_list<call_stack<c0>, call_stack<c1>>>::type
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
          , type_list<
                fake_dependency<fake_scope<>, std::string, s, no_name, type_list<call_stack<c0>>>::type
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
          , type_list<
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
          , type_list<
                fake_dependency<
                    fake_scope<>
                  , cif0if1
                  , cif0if1
                  , no_name
                  , type_list<>
                  , bindings::detail::requires_<
                        bindings::type_traits::is_required_priority
                      , bindings::type_traits::is_required_type<any_of<if0, if1>>
                      , bindings::detail::when_<>
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
          , type_list<>
        >::value
    ));
}

using scope_one_types = mpl::vector<
    pair<scopes::deduce, deduce<int>>
  , pair<scopes::unique<>, unique<int>>
  , pair<scopes::shared<>, shared<int>>
  , pair<scopes::session<>, session<int>>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_one, T, scope_one_types) {
    BOOST_CHECK((
        contains_all<
            typename T::second
          , type_list<
                typename fake_dependency<typename T::first, int>::type
            >
        >::value
    ));
}

using scope_many_types = mpl::vector<
    pair<scopes::deduce, deduce<int, double, float>>
  , pair<scopes::unique<>, unique<int, double, float>>
  , pair<scopes::shared<>, shared<int, double, float>>
  , pair<scopes::session<>, session<int, double, float>>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_many, T, scope_many_types) {
    BOOST_CHECK((
        contains_all<
            typename T::second
          , type_list<
                typename fake_dependency<typename T::first, int>::type
              , typename fake_dependency<typename T::first, double>::type
              , typename fake_dependency<typename T::first, float>::type
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
          , type_list<
                fake_dependency<fake_scope<>, i, impl, no_name, type_list<call_stack<c0, c1>>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_deduce_with_deduced_interface) {
    typedef fake_dependency<scopes::deduce, impl, impl>::type expected;

    BOOST_CHECK((
        contains_all<
            type_list<
                bind<impl>::type
              , bind<impl, impl>::type
            >
          , type_list<expected, expected>
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            deduce<impl>::type
          , type_list<expected>
        >::value
    ));

    BOOST_CHECK((
        contains_all<
            deduce<bind<impl>>::type
          , type_list<expected>
        >::value
    ));
}

} // namespace di
} // namespace boost

