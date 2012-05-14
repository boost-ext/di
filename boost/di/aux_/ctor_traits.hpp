//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CTOR_TRAITS_HPP
#define BOOST_DI_AUX_CTOR_TRAITS_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/bool.hpp>
#include "boost/di/aux_/has_traits.hpp"
#include "boost/di/ctor.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

template<typename T>
struct ctor_traits
{
    static void ctor(); //trivial ctor
};

namespace aux_ {

template<typename T, typename = void>
struct ctor_traits
    : function_types::parameter_types<
          BOOST_TYPEOF_TPL(di::ctor_traits<T>::ctor)
      >::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_CTOR_UNIQUE_NAME)<T> >::type>
    : function_types::parameter_types<
          BOOST_TYPEOF_TPL(T::BOOST_DI_CTOR_UNIQUE_NAME::ctor)
      >::type
{ };

} // namespace aux_
} // namespace di
} // namespace boost

#endif

