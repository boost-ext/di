//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(BOOST_DI_PREPROCESSED)
	#include "boost/di/aux_/preprocessed/type_traits/is_same_base_of.hpp"
#endif

#ifndef BOOST_DI_TYPE_TRAITS_IS_SAME_BASE_OF_HPP
#define BOOST_DI_TYPE_TRAITS_IS_SAME_BASE_OF_HPP

#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>

namespace boost {
namespace di {
namespace type_traits {

template<typename T, typename U = mpl::_1>
struct is_same_base_of
    : mpl::or_<
          is_base_of<U, T>
        , is_same<U, T>
      >
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

