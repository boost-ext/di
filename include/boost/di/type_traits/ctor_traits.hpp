//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/type_traits/has_injector.hpp"
#include "boost/di/type_traits/function_traits.hpp"

#include <string>
#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>
#include <type_traits>

namespace boost {
namespace di {

template<typename T>
struct ctor_traits {
    using type = decltype(
        hana::second(
            hana::foldl(
                hana::range_c<int, 1, BOOST_DI_CFG_CTOR_LIMIT_SIZE>
              , hana::pair(hana::type_list<core::any_type<T>>, hana::type_list<>)
              , [](auto result, auto) {
                    auto is_construcrible =
                        hana::partial(hana::flip(hana::unpack), hana::trait<std::is_constructible>);

                    auto which = hana::if_(
                        is_construcrible(hana::cons(hana::type<T>, hana::first(result)))
                      , hana::first
                      , hana::second
                    );

                    return hana::pair(
                        hana::snoc(hana::first(result), hana::type<core::any_type<T>>)
                      , which(result)
                    );
                }
            )
        )
    );
};

template<typename T>
struct ctor_traits<std::basic_string<T> > // basic_string ctors are ambiguous
{
    static void BOOST_DI_INJECTOR();
};

namespace type_traits {

template<typename T, typename = void>
struct ctor_traits
    : di::ctor_traits<T>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T> > >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<T> >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&T::BOOST_DI_INJECTOR)>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

