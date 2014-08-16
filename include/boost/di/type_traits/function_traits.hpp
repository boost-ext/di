//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_DETAIL_FUNCTION_TRAITS_HPP
#define BOOST_DI_AUX_DETAIL_FUNCTION_TRAITS_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename>
struct function_traits;

template<typename R, typename... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using type = type_list<TArgs...>;
};

template<typename R, typename T, typename... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using base_type = T;
    using type = type_list<TArgs...>;
};

template<typename R, typename T, typename... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using base_type = T;
    using type = type_list<TArgs...>;
};

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

