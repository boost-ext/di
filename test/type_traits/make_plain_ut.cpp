//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/make_plain.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/named.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace type_traits {

typedef mpl::vector<int, a> make_plain_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(basic, T, make_plain_t) {
    BOOST_CHECK((is_same<T, typename make_plain<T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<T*>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T*>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<T&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<aux::shared_ptr<T>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const aux::shared_ptr<T>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<aux::shared_ptr<T>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<volatile T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const named<T, mpl::_1>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<named<T, mpl::_1>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<named<aux::shared_ptr<T>, mpl::_1>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<named<const aux::shared_ptr<T>&, mpl::_1>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<aux::shared_ptr<named<const aux::shared_ptr<T>&, mpl::_1>>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const aux::shared_ptr<named<const aux::shared_ptr<T>&, mpl::_1>>&>::type>::value));
}

#if defined(BOOST_HAS_RVALUE_REFERENCES)

BOOST_AUTO_TEST_CASE_TEMPLATE(rvalue_references, T, make_plain_t) {
    BOOST_CHECK((is_same<T, typename make_plain<T&&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T&&>::type>::value));
}

#endif

} // namespace type_traits
} // namespace di
} // namespace boost

