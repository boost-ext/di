//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_NAME_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_NAME_HPP

#include "boost/di/type_traits/remove_accessors.hpp"

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

template<typename T, typename = void>
struct get_name
{
    struct no_name { };
    typedef no_name type;
};

template<typename T>
struct get_name<T, typename enable_if<
    has_name<typename di::type_traits::remove_accessors<T>::type> >::type
>
{
    typedef typename di::type_traits::remove_accessors<T>::type::name type;
};

template<typename TName>
struct name
{
    template<typename T, typename, typename>
    struct apply
        : is_same<typename get_name<T>::type, TName>
    { };

    int operator()(const std::type_info*, const std::type_info* name, const std::vector<const std::type_info*>&) const {
        return &typeid(TName) == name;
    }
};

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

#endif

