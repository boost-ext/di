//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ARRAY_HPP
#define BOOST_DI_CORE_ARRAY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/type_traits/rebind_traits.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class T, int N>
struct array__ { T array_[N]; };

template<class T, class... Ts>
struct array : array__<typename T::value_type, sizeof...(Ts)>, T {
    using value_type = typename T::value_type;
	using array__<value_type, sizeof...(Ts)>::array_;
    using boost_di_inject__ = aux::type_list<self>;

    template<class TInjector>
    explicit array(const TInjector& injector)
        : array__<value_type, sizeof...(Ts)>{{
            *static_cast<const core::injector__<TInjector>&>(injector).
                create_successful_impl(aux::type<type_traits::rebind_traits_t<value_type, Ts>>{})...}
         }
        , T(std::move_iterator<value_type*>(array_)
          , std::move_iterator<value_type*>(array_ + sizeof...(Ts)))
    { }
};

template<class... Ts>
struct array<_, Ts...> { };

}}}} // boost::di::v1::core

#endif

