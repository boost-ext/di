//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQ_NAME_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQ_NAME_HPP

#include "boost/di/aux_/preprocessed/concepts/type_traits/is_req_name.hpp"
#include "boost/di/aux_/preprocessed/type_traits/remove_accessors.hpp"
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/times.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

template<typename T, typename, typename = void>
struct is_req_name_impl
    : mpl::false_
{ };

template<typename T, typename TName>
struct is_req_name_impl<T, TName, typename enable_if<has_name<T> >::type>
    : is_same<typename T::name, TName>
{ };

template<typename TName, typename U = mpl::_1>
struct is_req_name
    : mpl::times<mpl::int_<5>,
     is_req_name_impl<
          typename di::type_traits::remove_accessors<U>::type
        , TName
      >
      >
{ };

}
}
}
}

#endif

