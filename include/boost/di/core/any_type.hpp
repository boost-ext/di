//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

#include <memory>
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/named.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_TYPE(is_ref);

template<class TParent = aux::none_t, class TInjector = aux::none_t>
struct any_type {
    template<class T>
    using is_not_same_t = std::enable_if_t<
        !std::is_same<aux::make_plain_t<T>, aux::make_plain_t<TParent>>{} &&
        !std::is_base_of<aux::make_plain_t<TParent>, aux::make_plain_t<T>>{}
    >;

    template<class T>
    using dependency = typename std::remove_reference_t<
        decltype(binder::resolve<T>((TInjector*)nullptr))
    >;

    template<class T>
    using is_ref_t = std::enable_if_t<
        std::is_same<TInjector, aux::none_t>{} || has_is_ref<dependency<T>>{}
    >;

    template<class T, class = is_not_same_t<T>>
    operator T() noexcept {
        return injector_.template create<T, TParent>();
    }

    template<class T, class = is_not_same_t<T>, class = is_ref_t<T>>
    operator T&() const noexcept {
        return injector_.template create<T&, TParent>();
    }

    template<class T, class = is_not_same_t<T>, class = is_ref_t<T>>
    operator const T&() const noexcept {
        return reinterpret_cast<const T&>(injector_.template create<const T&, TParent>().operator const typename T::named_type&());
    }

    const TInjector& injector_;
};

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

}}} // boost::di::core

#endif

