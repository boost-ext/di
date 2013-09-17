//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_MAKE_PLAIN_HPP
#define BOOST_DI_TYPE_TRAITS_MAKE_PLAIN_HPP

#include <boost/shared_ptr.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
namespace type_traits {

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

template<typename T>
struct remove_accessors
    : remove_cv<
        typename remove_pointer<
            typename remove_reference<T>::type
        >::type
      >
{ };

template<typename T, typename = void>
struct deref_element_type
{
    typedef T type;
};

template<typename T>
struct deref_element_type<T, typename enable_if<has_element_type<T> >::type>
{
    typedef typename T::element_type type;
};

template<typename TElement>
struct make_plain
    : deref_element_type<
        typename remove_accessors<
            typename deref_element_type<
                typename remove_accessors<TElement>::type
            >::type
        >::type
    >
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

