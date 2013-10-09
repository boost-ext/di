//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts.hpp"
#include <boost/mpl/int.hpp>

#include <boost/test/unit_test.hpp>

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
                fake_dependency_base_of<fake_scope<>, int, mpl::int_<i>>::type
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
                fake_dependency_base_of<fake_scope<>, named<int, void>, mpl::int_<i>>::type
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
                fake_dependency_base_of<fake_scope<>, int, mpl::int_<i>, c0>::type
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
                fake_dependency_base_of<fake_scope<>, named<int, void>, mpl::int_<i>, c0>::type
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
                fake_dependency_base_of<fake_scope<>, named<int, void>, mpl::int_<i>, c0>::type
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
                fake_dependency_base_of<fake_scope<>, named<std::string, void>, s>::type
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
                fake_dependency_base_of<fake_scope<>, named<std::string, void>, s, c0, c1>::type
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
                fake_dependency_base_of<fake_scope<>, named<std::string, void>, s, c0, c1>::type
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
                fake_dependency_base_of<fake_scope<>, std::string, s, c0>::type
            >
        >::value
    ));
}

#if 0
BOOST_AUTO_TEST_CASE(bind_vector_value) {
    BOOST_CHECK((
        contains_all<
            bind_vector<int, int_<42>>
          , mpl::vector<
                fake_dependency_base_of<scopes::deduce, std::vector<int>, make_vector<int, mpl::vector<int_<42>>>>::type
              //, fake_dependency_base_of<fake_scope<>, std::vector<boost::shared_ptr<int>>, make_vector<boost::shared_ptr<int>, boost::shared_ptr<int_<42>>>>::type
            >
        >::value
    ));
}
#endif

BOOST_AUTO_TEST_CASE(scope_deduce_empty) {
    BOOST_CHECK((
        contains_all<
            deduce<>::type
          , mpl::vector<>
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_deduce_one) {
    BOOST_CHECK((
        contains_all<
            deduce<int>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::deduce, int>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_deduce_many) {
    BOOST_CHECK((
        contains_all<
            deduce<int, double, float>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::deduce, int>::type
              , fake_dependency_base_of<scopes::deduce, double>::type
              , fake_dependency_base_of<scopes::deduce, float>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_per_request_empty) {
    BOOST_CHECK((
        contains_all<
            per_request<>::type
          , mpl::vector<>
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_per_request_one) {
    BOOST_CHECK((
        contains_all<
            per_request<int>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::per_request<>, int>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_per_request_many) {
    BOOST_CHECK((
        contains_all<
            per_request<int, double, float>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::per_request<>, int>::type
              , fake_dependency_base_of<scopes::per_request<>, double>::type
              , fake_dependency_base_of<scopes::per_request<>, float>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_singleton_empty) {
    BOOST_CHECK((
        contains_all<
            singleton<>::type
          , mpl::vector<>
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_singleton_one) {
    BOOST_CHECK((
        contains_all<
            singleton<int>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, int>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_singleton_many) {
    BOOST_CHECK((
        contains_all<
            singleton<int, double, float>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::singleton<>, int>::type
              , fake_dependency_base_of<scopes::singleton<>, double>::type
              , fake_dependency_base_of<scopes::singleton<>, float>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_session_empty) {
    BOOST_CHECK((
        contains_all<
            session<>::type
          , mpl::vector<>
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_session_one) {
    BOOST_CHECK((
        contains_all<
            session<int>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::session<>, int>::type
            >
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(scope_session_many) {
    BOOST_CHECK((
        contains_all<
            session<int, double, float>::type
          , mpl::vector<
                fake_dependency_base_of<scopes::session<>, int>::type
              , fake_dependency_base_of<scopes::session<>, double>::type
              , fake_dependency_base_of<scopes::session<>, float>::type
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
                fake_dependency_base_of<fake_scope<>, i, impl, call_stack<c0, c1>>::type
            >
        >::value
    ));
}

} // namespace di
} // namespace boost

