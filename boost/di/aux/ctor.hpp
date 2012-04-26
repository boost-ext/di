//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CTOR_HPP
#define BOOST_DI_AUX_CTOR_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>
#include "boost/di/ctor.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace aux {

namespace detail {
BOOST_MPL_HAS_XXX_TRAIT_DEF(BOOST_DI_CTOR_UNIQUE_NAME)
} // namespace detail

template<typename T, typename = void>
struct ctor
    : function_types::parameter_types<
          BOOST_TYPEOF_TPL(ctor_traits<T>::ctor)
      >::type
{ };

template<typename T>
struct ctor<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CTOR_UNIQUE_NAME)<T> >::type>
    : function_types::parameter_types<
          BOOST_TYPEOF_TPL(T::BOOST_DI_CTOR_UNIQUE_NAME::ctor)
      >::type
{ };

} // namespace aux
} // namespace di
} // namespace boost

#endif

