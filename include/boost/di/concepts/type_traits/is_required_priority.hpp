//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQUIRED_PRIORITY_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQUIRED_PRIORITY_HPP

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/plus.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

struct is_required_priority
{
    template<typename T>
    struct apply
        : mpl::plus<
              mpl::int_<1>
            , typename T::scope::priority // lowest = 0, highest = N
          >
    { };
};

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

#endif

