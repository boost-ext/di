//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/type_traits/has_injector.hpp"
#include "boost/di/type_traits/function_traits.hpp"

#include <string>
#include <boost/preprocessor/cat.hpp>

namespace boost {
namespace di {

template<typename T, unsigned... Args>
struct ctor_traits_impl<T, seq<Args...>>
    : longest<typename genn<T, core::any_type<T>, Args>::type...>
{ };

template<typename T>
struct ctor_traits
    : ctor_traits_impl<T, typename gen_seq<BOOST_DI_CFG_CTOR_LIMIT_SIZE>::type>
{ };

template<typename T>
struct ctor_traits<std::basic_string<T>> {
    static void BOOST_DI_INJECTOR();
};

namespace type_traits {

template<typename T, typename = void>
struct ctor_traits
    : di::ctor_traits<T>::type
{ };

template<typename T>
struct ctor_traits<T, typename std::enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::value>::type>
    : function_traits<decltype(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, typename std::enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<T>::value>::type>
    : function_traits<decltype(&T::BOOST_DI_INJECTOR)>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

