//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_MAKE_PLAIN_HPP
#define BOOST_DI_AUX_MAKE_PLAIN_HPP

#include <boost/shared_ptr.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/is_sequence.hpp>
#include "boost/di/aux_/has_traits.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace aux_ {

namespace detail {

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

} // namespace detail

template<typename TElement>
struct make_plain
    : detail::deref_element_type<
        typename detail::remove_accessors<
            typename detail::deref_element_type<
                typename detail::remove_accessors<TElement>::type
            >::type
        >::type
    >
{ };

} // namespace aux_
} // namespace di
} // namespace boost

#endif

