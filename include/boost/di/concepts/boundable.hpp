//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_BOUNDABLE_HPP
#define BOOST_DI_CONCEPTS_BOUNDABLE_HPP

#include <type_traits>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/pool.hpp"

namespace boost { namespace di { namespace concepts {

template<class T>
struct unique {
    using type = aux::pair<
        aux::pair<typename T::expected, typename T::name>
      , std::integral_constant<bool, T::scope::priority>
    >;
};

std::false_type boundable_impl(...);

template<class... Ts>
auto boundable_impl(aux::type_list<Ts...>&&) ->
    aux::always<typename aux::inherit<typename unique<Ts>::type...>::type>;

template<class I, class T>
auto boundable_impl(I&&, T&&) -> std::integral_constant<bool,
    std::is_convertible<T, I>{} || std::is_base_of<I, T>{}
>;

template<class T, class... Ts>
auto boundable_impl(aux::type_list<Ts...>&&, T&&) ->
    std::integral_constant<
        bool
      , !std::is_same<
            aux::bool_list<aux::never<Ts>{}...>
          , aux::bool_list<std::is_base_of<Ts, T>{}...>
        >{}
    >;

template<class... Ts>
decltype(boundable_impl(std::declval<Ts>()...)) boundable{};

}}} // boost::di::concepts

#endif

