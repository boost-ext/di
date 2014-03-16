//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/dependency.hpp"

#include <typeinfo>
#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>

#include "common/fakes/fake_scope.hpp"

namespace boost {
namespace di {
namespace concepts {

BOOST_AUTO_TEST_CASE(ctor) {
    dependency<fake_scope<>, int> dep(42);
}

BOOST_AUTO_TEST_CASE(default_value) {
    typedef dependency<fake_scope<>, int> dependency_t;

    BOOST_CHECK((is_same<
        detail::requires<
            concepts::type_traits::priority
          , concepts::type_traits::type<int>
        >
      , dependency_t::bind
    >::value));
}

BOOST_AUTO_TEST_CASE(rebind_scope) {
    BOOST_CHECK((
        is_same<
            dependency<
                fake_scope<1>
              , int
              , int
              , detail::requires<is_same<mpl::_1, int>>
            >
          , dependency<
                mpl::_1
              , int
              , int
              , detail::requires<is_same<mpl::_1, int>>
            >::rebind<fake_scope<1>>::other
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(to_value_arithmetic) {
    using expected = scopes::external<convertibles::value>;
    using given = decltype(dependency<fake_scope<>, int>::to(int()))::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_value_enum) {
    enum e { };
    using expected = scopes::external<convertibles::value>;
    using given = decltype(dependency<fake_scope<>, e>::to(e()))::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_value_text) {
    using expected = scopes::external<convertibles::value>;
    using given = decltype(dependency<fake_scope<>, std::string>::to(std::string()))::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_const_ref) {
    struct c { } c_;
    using expected = scopes::external<convertibles::reference>;
    using given = decltype(dependency<fake_scope<>, c>::to(c_))::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_ref) {
    struct c { } c_;
    c& c_ref_ = c_;
    using expected = scopes::external<convertibles::reference>;
    using given = decltype(dependency<fake_scope<>, c>::to(c_ref_))::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    using expected = scopes::external<convertibles::shared>;
    using given = decltype(dependency<fake_scope<>, int>::to(aux::shared_ptr<int>()))::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_lambda_shared_ptr) {
    using expected = scopes::external<convertibles::shared>;
    auto given = dependency<fake_scope<>, int>::to([]{ return aux::shared_ptr<int>(); });
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(decltype(given)::scope));
}

} // namespace concepts
} // namespace di
} // namespace boost

