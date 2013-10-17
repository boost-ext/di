//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts.hpp"
#include <boost/mpl/int.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/pair.hpp>

#include "data.hpp"
#include "fake_dependency_base_of.hpp"
#include "fake_scope.hpp"
#include "contains_all.hpp"

namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(bind_impl) {
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind<i, impl>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, i, impl>::type
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
                fake_dependency_base_of<fake_scope<>, int, int_<i>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_in_name) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::in_name<void>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, int, int_<i>, void>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_in_call) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::in_call<c0>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, int, int_<i>, void, c0>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_in_name_in_call) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::in_name<void>::in_call<c0>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, int, int_<i>, void, c0>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_int_value_in_call_in_name) {
    const int i = 42;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_int<i>::in_call<c0>::in_name<void>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, int, int_<i>, void, c0>::type
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
                fake_dependency_base_of<fake_scope<>, std::string, s>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_in_name) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::in_name<void>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, std::string, s, void>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_in_name_in_call) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::in_name<void>::in_call<c0, c1>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, std::string, s, void, c0, c1>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_in_call_in_name) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::in_call<c0, c1>::in_name<void>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, std::string, s, void, c0, c1>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(bind_string_value_in_call) {
    using s = mpl::string<'s'>;
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind_string<s>::in_call<c0>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, std::string, s, void, c0>::type
            >
        >::value
    ));
}

using scope_empty_types = mpl::vector<
    deduce<>
  , unique<>
  , thread<>
  , shared<>
  , singleton<>
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
    mpl::pair< scopes::deduce,                      deduce<int>        >
  , mpl::pair< scopes::unique<>,               unique<int>   >
  , mpl::pair< scopes::thread<scopes::deduce>,  thread<int>    >
  , mpl::pair< scopes::shared<>,                    shared<int>        >
  , mpl::pair< scopes::singleton<>,                 singleton<int>     >
  , mpl::pair< scopes::session<>,                   session<int>       >
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_one, T, scope_one_types) {
    BOOST_CHECK((
        contains_all<
            typename mpl::second<T>::type
          , mpl::vector<
                typename fake_dependency_base_of<typename mpl::first<T>::type, int>::type
            >
        >::value
    ));
}

using scope_many_types = mpl::vector<
    mpl::pair< scopes::deduce,                      deduce<int, double, float>        >
  , mpl::pair< scopes::unique<>,               unique<int, double, float>   >
  , mpl::pair< scopes::thread<scopes::deduce>,  thread<int, double, float>    >
  , mpl::pair< scopes::shared<>,                    shared<int, double, float>        >
  , mpl::pair< scopes::singleton<>,                 singleton<int, double, float>     >
  , mpl::pair< scopes::session<>,                   session<int, double, float>       >
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(scope_many, T, scope_many_types) {
    BOOST_CHECK((
        contains_all<
            typename mpl::second<T>::type
          , mpl::vector<
                typename fake_dependency_base_of<typename mpl::first<T>::type, int>::type
              , typename fake_dependency_base_of<typename mpl::first<T>::type, double>::type
              , typename fake_dependency_base_of<typename mpl::first<T>::type, float>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_with_call_stack) {
    BOOST_CHECK((
        contains_all<
            scope<fake_scope<>>::bind<
                bind<i, impl>::in_call<call_stack<c0, c1>>::type
            >
          , mpl::vector<
                fake_dependency_base_of<fake_scope<>, i, impl, void, call_stack<c0, c1>>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(thread_mix) {
    BOOST_CHECK((
        contains_all<
            thread<
                int
              , bind<if0, c0if0>
              , unique<double>
              , shared<float, char>
              , unique<bind<c1>::in_name<float> >
              , deduce<c2>::in_call<double>
            >::type
          , mpl::vector<
                fake_dependency_base_of<scopes::thread<scopes::deduce>, int>::type
              , fake_dependency_base_of<scopes::thread<scopes::deduce>, if0, c0if0>::type
              , fake_dependency_base_of<scopes::thread<scopes::unique<>>, double>::type
              , fake_dependency_base_of<scopes::thread<scopes::shared<>>, float>::type
              , fake_dependency_base_of<scopes::thread<scopes::shared<>>, char>::type
              , fake_dependency_base_of<scopes::thread<scopes::unique<>>, c1, c1, float>::type
              , fake_dependency_base_of<scopes::thread<scopes::deduce>, c2, c2, void, double>::type
            >
        >::value
    ));
}

} // namespace di
} // namespace boost

