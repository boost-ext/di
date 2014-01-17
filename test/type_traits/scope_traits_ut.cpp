//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/scope_traits.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace type_traits {

struct c { };

BOOST_AUTO_TEST_CASE(basic) {
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<int>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<int>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<c>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<c>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<const int&>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<const int&>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<int*>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<int*>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<const int*>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<const int*>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<aux::auto_ptr<int>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<aux::auto_ptr<int>>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<aux::shared_ptr<int>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<named<aux::shared_ptr<int>>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<aux::weak_ptr<int>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<named<aux::weak_ptr<int>>>::type>::value));
    BOOST_CHECK((is_same<scopes::external<>, scope_traits<int&>::type>::value));
    BOOST_CHECK((is_same<scopes::external<>, scope_traits<named<int&>>::type>::value));

#if !defined(BOOST_NO_CXX11_SMART_PTR)
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<aux::unique_ptr<int>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<aux::unique_ptr<int>>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<aux::shared_ptr<int>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<named<aux::shared_ptr<int>>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<aux::weak_ptr<int>>::type>::value));
    BOOST_CHECK((is_same<scopes::shared<>, scope_traits<named<aux::weak_ptr<int>>>::type>::value));
#endif

#if !defined(BOOST_NO_RVALUE_REFERENCES)
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<int&&>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<int&&>>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<const int&&>::type>::value));
    BOOST_CHECK((is_same<scopes::unique<>, scope_traits<named<const int&&>>::type>::value));
#endif
}

} // namespace type_traits
} // namespace di
} // namespace boost

