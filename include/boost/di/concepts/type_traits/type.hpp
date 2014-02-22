//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_TYPE_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_TYPE_HPP

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/is_same_base_of.hpp"

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/or.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

template<typename T>
struct type
{
    template<typename U, typename, typename>
    struct apply
        : di::type_traits::is_same_base_of<
              T
            , typename di::type_traits::make_plain<U>::type
          >
    { };

    int operator()(const std::type_info* t, const std::type_info*, const std::vector<const std::type_info*>&) const {
        return &typeid(T) == t;
    }
};

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

#endif

