//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class T, class TParent>
using is_not_same_t = BOOST_DI_REQUIRES(!aux::is_same_or_base_of<T, TParent>::value);

template<class T, class TInjector>
struct is_referable_impl {
    static constexpr auto value =
        dependency__<binder::resolve_t<TInjector, T>>::template is_referable<T>::value;
};

template<class T, class TInjector>
using is_referable_t = BOOST_DI_REQUIRES(is_referable_impl<T, TInjector>::value);

template<class T, class TInjector, class TError>
struct is_creatable_impl {
    static constexpr auto value = injector__<TInjector>::template is_creatable<T>::value;
};

template<class T, class TInjector>
struct is_creatable_impl<T, TInjector, aux::false_type> {
    static constexpr auto value = true;
};

template<class T, class TInjector, class TError>
using is_creatable_t = BOOST_DI_REQUIRES(is_creatable_impl<T, TInjector, TError>::value);

template<class TParent, class TInjector, class TError = aux::false_type>
struct any_type {
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_creatable_t<T, TInjector, TError>
    > operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
    }

    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>
           , class = is_creatable_t<const T&, TInjector, TError>
    > operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
    }

    const TInjector& injector_;
};

template<class TParent, class TInjector, class TError = aux::false_type>
struct any_type_ref {
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_creatable_t<T, TInjector, TError>
    > operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
    }

    #if defined(__GNUC__) // __pph__
        template<class T
               , class = is_not_same_t<T, TParent>
               , class = is_referable_t<T&&, TInjector>
               , class = is_creatable_t<T&&, TInjector, TError>
        > operator T&&() const {
            return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&&>{});
        }
    #endif // __pph__

    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<T&, TInjector>
           , class = is_creatable_t<T&, TInjector, TError>
    > operator T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&>{});
    }

    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>
           , class = is_creatable_t<const T&, TInjector, TError>
    > operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
    }

    const TInjector& injector_;
};

namespace successful {

template<class TParent, class TInjector>
struct any_type {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T>{});
    }

    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>
    > operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<const T&>{});
    }

    const TInjector& injector_;
};

template<class TParent, class TInjector>
struct any_type_ref {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T>{});
    }

    #if defined(__GNUC__) // __pph__
        template<class T
               , class = is_not_same_t<T, TParent>
               , class = is_referable_t<T&&, TInjector>
        > operator T&&() const {
            return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T&&>{});
        }
    #endif // __pph__

    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<T&, TInjector>
    > operator T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T&>{});
    }

    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>
    > operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<const T&>{});
    }

    const TInjector& injector_;
};

} // successful

template<class TParent>
struct any_type_fwd {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T();
};

template<class TParent>
struct any_type_ref_fwd {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T();

    template<class T, class = is_not_same_t<T, TParent>>
    operator T&() const;

    #if defined(__GNUC__) // __pph__
        template<class T, class = is_not_same_t<T, TParent>>
        operator T&&() const;
    #endif // __pph__

    template<class T, class = is_not_same_t<T, TParent>>
    operator const T&() const;
};

}}}} // boost::di::v1::core

#endif

