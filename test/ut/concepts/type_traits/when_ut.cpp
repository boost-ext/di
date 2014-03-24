//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/type_traits/when.hpp"

#include <boost/test/unit_test.hpp>

#include "common/fakes/fake_scope.hpp"
#include "common/fakes/fake_concept.hpp"

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

BOOST_AUTO_TEST_CASE(empty_context) {
    BOOST_CHECK_EQUAL(1, (when<mpl::vector0<>>::apply<void, mpl::vector0<>, fake_scope<>>::type::value));
}

BOOST_AUTO_TEST_CASE(one_concept_fail) {
    BOOST_CHECK_EQUAL(0, (
        when<
            mpl::vector<
                fake_concept<0>
            >
        >::apply<void, mpl::vector0<>, fake_scope<>>::type::value)
    );
}

BOOST_AUTO_TEST_CASE(one_concept_pass) {
    BOOST_CHECK_EQUAL(1, (
        when<
            mpl::vector<
                fake_concept<1>
            >
        >::apply<void, mpl::vector0<>, fake_scope<>>::type::value)
    );
}

BOOST_AUTO_TEST_CASE(many_concepts_max_element) {
    BOOST_CHECK_EQUAL(8, (
        when<
            mpl::vector<
                fake_concept<5>
              , fake_concept<8>
              , fake_concept<1>
              , fake_concept<3>
            >
        >::apply<void, mpl::vector0<>, fake_scope<>>::type::value)
    );
}

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

