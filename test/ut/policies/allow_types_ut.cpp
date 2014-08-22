//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/allow_types.hpp"

#include <type_traits>
#include <boost/test/unit_test.hpp>

#include "boost/di/scopes/unique.hpp"
#include "common/fakes/fake_data.hpp"

namespace boost {
namespace di {
namespace policies {

template<
    typename T
  , typename TResolve = std::false_type
>
struct fake_dependency {
    using type = T;

    struct resolved {
        using type =  resolved;

        template<typename>
        struct rebind {
            using other = resolved;
        };
    };

    struct binder {
        template<
            typename
          , typename
          , typename TDefault
        >
        using resolve = typename std::conditional<
            TResolve::value
          , resolved
          , TDefault
        >::type;
    };
};

BOOST_AUTO_TEST_CASE(dummy) {
    allow_types(is_copy() || is_ptr()).assert_policy<fake_data<int*, type_list<>, scopes::unique<>>>();
    allow_types(is_type<int>()).assert_policy<fake_dependency<int, std::true_type>>();
    allow_types(is_bound()).assert_policy<fake_dependency<int, std::true_type>>();
}

} // namespace policies
} // namespace di
} // namespace boost

