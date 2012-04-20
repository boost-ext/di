//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>
#include "boost/di/named.hpp"
#include "boost/di/aux/utility.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace aux {
namespace ut {

typedef mpl::vector<int, a> make_plain_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(utility_make_plain, T, make_plain_t)
{
    BOOST_CHECK((is_same<T, typename make_plain<T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<T*>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T*>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<T&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<shared_ptr<T> >::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const shared_ptr<T>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<shared_ptr<T>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<volatile T>::type>::value));
    BOOST_CHECK((is_same<named<T, _1>, typename make_plain<named<T, _1> >::type>::value));
    BOOST_CHECK((is_same<named<T, _1>, typename make_plain<named<shared_ptr<T>, _1> >::type>::value));
    BOOST_CHECK((is_same<named<T, _1>, typename make_plain<named<const shared_ptr<T>&, _1> >::type>::value));
    BOOST_CHECK((is_same<named<T, _1>, typename make_plain<shared_ptr< named<const shared_ptr<T>&, _1> > >::type>::value));
    BOOST_CHECK((is_same<named<T, _1>, typename make_plain<const shared_ptr< named<const shared_ptr<T>&, _1> >&>::type>::value));
}

} // namespace ut
} // namespace aux
} // namespace di
} // namespace boost

