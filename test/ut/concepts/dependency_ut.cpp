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

#include "boost/di/aux_/config.hpp"

#include "common/fakes/fake_scope.hpp"

namespace boost {
namespace di {
namespace concepts {

BOOST_AUTO_TEST_CASE(ctor) {
    dependency<fake_scope<>, int> dep(42);
}

BOOST_AUTO_TEST_CASE(default_value) {
    using dependency_t = dependency<fake_scope<>, int>;

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
    using expected = scopes::external<wrappers::value>;
    using external = decltype(dependency<fake_scope<>, int>::to(int()));
    using given = external::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_value_enum) {
    enum e { };
    using expected = scopes::external<wrappers::value>;
    using external = decltype(dependency<fake_scope<>, e>::to(e()));
    using given = external::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_value_text) {
    using expected = scopes::external<wrappers::value>;
    using external = decltype(dependency<fake_scope<>, std::string>::to(std::string()));
    using given = external::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

/*BOOST_AUTO_TEST_CASE(to_const_ref) {*/
    //struct c { };
    //using expected = scopes::external<wrappers::reference>;
    //using external = decltype(dependency<fake_scope<>, c>::to(c()));
    //using given = external::scope;
    //BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
//}

//BOOST_AUTO_TEST_CASE(to_ref) {
    //struct c { } c_;
    //c& c_ref_ = c_;
    //(void)c_ref_;
    //using expected = scopes::external<wrappers::reference>;
    //using external = decltype(dependency<fake_scope<>, c>::to(c_ref_));
    //using given = external::scope;
    //BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
/*}*/

BOOST_AUTO_TEST_CASE(to_shared_ptr) {
    using expected = scopes::external<wrappers::shared>;
    using external = decltype(dependency<fake_scope<>, int>::to(aux::shared_ptr<int>()));
    using given = external::scope;
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(given));
}

BOOST_AUTO_TEST_CASE(to_lambda_shared_ptr) {
    using expected = scopes::external<wrappers::shared>;
    auto given = dependency<fake_scope<>, int>::to([]{ return aux::shared_ptr<int>(); });
    using external = decltype(given);
    BOOST_CHECK_EQUAL(&typeid(expected), &typeid(external::scope));
}

} // namespace concepts
} // namespace di
} // namespace boost

