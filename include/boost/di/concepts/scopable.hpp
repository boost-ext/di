//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPABLE_HPP
#define BOOST_DI_CONCEPTS_SCOPABLE_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di { namespace concepts {

template<class T>
struct provider {
    template<class TMemory = type_traits::heap>
    std::conditional_t<std::is_same<TMemory, type_traits::stack>::value, T, T*>
    try_get(const TMemory& = {}) const;

    template<class TMemory = type_traits::heap>
    T* get(const TMemory& = {}) const {
        return nullptr;
    }
};

std::false_type scopable_impl(...);

template<class T>
auto scopable_impl(T&&) -> aux::is_valid_expr<
    decltype(std::declval<typename T::template scope<_, _>>().template create<_>(provider<_>{}))
  , decltype(std::declval<typename T::template scope<_, _>>().template try_create<_>(provider<_>{}))
>;

template<class T>
using scopable =
    BOOST_DI_WKND(BOOST_DI_MSVC)(std::true_type)
    BOOST_DI_WKND_NOT(BOOST_DI_MSVC)(decltype(scopable_impl(std::declval<T>())));

}}} // boost::di::concepts

#endif

