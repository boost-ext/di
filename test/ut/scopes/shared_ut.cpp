//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/shared.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>

#include "boost/di/aux_/memory.hpp"

#include "common/fakes/fake_convertible.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_AUTO_TEST_CASE(create) {
    shared<>::scope<int> shared_;

    BOOST_CHECK((
        (shared_.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
        ==
        (shared_.create(type_traits::is_runtime<false>()))(type<aux::shared_ptr<int>>())
    ));
}

BOOST_AUTO_TEST_CASE(create_args) {
    shared<>::scope<c2> shared_;

    fake_convertible<int> i(0);
    fake_convertible<double> d(0.0);
    fake_convertible<char> c('0');

    BOOST_CHECK((
        (shared_.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
        ==
        (shared_.create<type_traits::is_runtime<false>, decltype(i), decltype(d), decltype(c)>(type_traits::is_runtime<false>(), i, d, c))(type<aux::shared_ptr<c2>>())
    ));
}

} // namespace scopes
} // namespace di
} // namespace boost

