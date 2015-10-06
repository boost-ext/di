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

template<class T>
struct array__<T, 0> { T array_[1]; };

template<class T, class... Ts>
struct array : array__<typename T::value_type, sizeof...(Ts)>, T {
    using value_type = typename T::value_type;
    using array_t = array__<value_type, sizeof...(Ts)>;
    using array_t::array_;
    using boost_di_inject__ = aux::type_list<self>;

    template<bool... Bs>
    struct and_ : aux::is_same<aux::type_list<typename aux::always<aux::integral_constant<bool, Bs>>::type...>, aux::type_list<aux::integral_constant<bool, Bs>...>> { };

    template<class TInjector>
    explicit array(const TInjector& injector)
        : array(injector, aux::true_type{})//, typename and_<core::injector__<TInjector>::template is_creatable<type_traits::rebind_traits_t<value_type, Ts>>::value...>::type{})
    { }

    template<class TInjector>
    array(const TInjector& injector, const aux::true_type&)
        : array_t{{
            *static_cast<const core::injector__<TInjector>&>(injector).
                create_successful_impl(aux::type<type_traits::rebind_traits_t<value_type, Ts>>{})...
         }}
        , T(std::move_iterator<value_type*>(array_)
          , std::move_iterator<value_type*>(array_ + sizeof...(Ts)))
    { }

    template<class TInjector>
    explicit array(const TInjector& injector, const aux::false_type&) {
        int _[]{0, (
            static_cast<const core::injector__<TInjector>&>(injector).
                create_impl(aux::type<type_traits::rebind_traits_t<value_type, Ts>>{})
        , 0)...}; (void)_;
    }
};

template<class T, std::size_t N, class... Ts>
struct array<std::array<T, N>, Ts...> : std::array<T, N> {
    using array_t = std::array<T, N>;
    using boost_di_inject__ = aux::type_list<self>;

    template<class TInjector>
    explicit array(const TInjector& injector)
        : array_t{{
            *static_cast<const core::injector__<TInjector>&>(injector).
                create_successful_impl(aux::type<type_traits::rebind_traits_t<T, Ts>>{})...
         }}
    { }
};

template<class T, class... Ts>
struct array<T*[], Ts...> { };

}}}} // boost::di::v1::core

#endif

