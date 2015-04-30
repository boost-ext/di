//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_TRANSFORM_HPP
#define BOOST_DI_CORE_TRANSFORM_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/scopes/exposed.hpp"

namespace boost { namespace di { namespace core {

template<class T, class = void>
struct get_deps {
    using type = typename T::deps;
};

template<class T>
struct get_deps<T, std::enable_if_t<has_configure<T>::value>> {
    using result_type = typename aux::function_traits<
        decltype(&T::configure)
    >::result_type;

    using type = typename result_type::deps;
};

template<
    class T
  , class = typename is_injector<T>::type
  , class = typename is_dependency<T>::type
> struct add_type_list;

template<class T, class TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = typename get_deps<T>::type;
};

template<class T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = aux::type_list<T>;
};

template<class T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = aux::type_list<dependency<scopes::exposed<>, T>>;
};

#if defined(_MSC_VER)
    template<class... Ts>
    struct transform : aux::join_t<typename add_type_list<Ts>::type...> { };

    template<class... Ts>
    using transform_t = typename transform<Ts...>::type;
#else
    template<class... Ts>
    using transform_t = aux::join_t<typename add_type_list<Ts>::type...>;
#endif

}}} // boost::di::core

#endif

