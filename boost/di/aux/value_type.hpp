//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_VALUE_TYPE_HPP
#define BOOST_DI_AUX_VALUE_TYPE_HPP

#include <string>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
namespace aux {

namespace detail {
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
} // namespace detail

template<typename T, typename = void>
struct value_type
{
    typedef T type;
};

template<>
struct value_type<std::string, void>
{
    typedef std::string type;
};

template<typename T>
struct value_type<T, typename enable_if<detail::has_value_type<T> >::type>
{
    typedef typename T::value_type type;
};

} // namespace aux
} // namespace di
} // namespace boost

#endif

