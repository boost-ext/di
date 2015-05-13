//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class T, class TParent>
using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

template<class T, class TInjector>
struct is_referable_impl {
    static constexpr auto value =
        std::remove_reference_t<decltype(binder::resolve<T>((TInjector*)nullptr))>::template
            is_referable<T>::value;
};

template<class T, class TInjector>
using is_referable = std::enable_if_t<is_referable_impl<T, TInjector>::value>;

template<class T, class TInjector, class E>
struct is_creatable_impl {
    static constexpr auto value = TInjector::template is_creatable<T>::value;
};

template<class T, class TInjector>
struct is_creatable_impl<T, TInjector, std::false_type> {
    static constexpr auto value = true;
};

template<class T, class TInjector, class E>
using is_creatable = std::enable_if_t<is_creatable_impl<T, TInjector, E>::value>;

template<class TParent, class TInjector, class E = std::false_type>
struct any_type {
    template<class T
           , class = is_not_same<T, TParent>
           , class = is_creatable<T, TInjector, E>
    > operator T() {
        return injector_.template create_impl(aux::type<T>{});
    }

    const TInjector& injector_;
};

template<class TParent, class TInjector, class E = std::false_type>
struct any_type_ref {
    template<class T
           , class = is_not_same<T, TParent>
           , class = is_creatable<T, TInjector, E>
    > operator T() {
        return injector_.template create_impl(aux::type<T>{});
    }

    BOOST_DI_WKND(BOOST_DI_GCC)(
        template<class T
               , class = is_not_same<T, TParent>
               , class = is_referable<T&&, TInjector>
               , class = is_creatable<T&&, TInjector, E>
        > operator T&&() const {
            return injector_.template create_impl(aux::type<T&&>{});
        }
    )()

    template<class T
           , class = is_not_same<T, TParent>
           , class = is_referable<T&, TInjector>
           , class = is_creatable<T&, TInjector, E>
    > operator T&() const {
        return injector_.template create_impl(aux::type<T&>{});
    }

    template<class T
           , class = is_not_same<T, TParent>
           , class = is_referable<const T&, TInjector>
           , class = is_creatable<const T&, TInjector, E>
    > operator const T&() const {
        return injector_.template create_impl(aux::type<const T&>{});
    }

    const TInjector& injector_;
};

namespace successful {

template<class TParent, class TInjector>
struct any_type {
    template<class T, class = is_not_same<T, TParent>>
    operator T() {
        return injector_.template create_successful_impl(aux::type<T>{});
    }

    const TInjector& injector_;
};

template<class TParent, class TInjector>
struct any_type_ref {
    template<class T, class = is_not_same<T, TParent>>
    operator T() {
        return injector_.template create_successful_impl(aux::type<T>{});
    }

    BOOST_DI_WKND(BOOST_DI_GCC)(
        template<class T
               , class = is_not_same<T, TParent>
               , class = is_referable<T&&, TInjector>
        > operator T&&() const {
            return injector_.template create_successful_impl(aux::type<T&&>{});
        }
    )()

    template<class T
           , class = is_not_same<T, TParent>
           , class = is_referable<T&, TInjector>
    > operator T&() const {
        return injector_.template create_successful_impl(aux::type<T&>{});
    }

    template<class T
           , class = is_not_same<T, TParent>
           , class = is_referable<const T&, TInjector>
    > operator const T&() const {
        return injector_.template create_successful_impl(aux::type<const T&>{});
    }

    const TInjector& injector_;
};

} // successful

template<class TParent>
struct any_type_fwd {
    template<class T, class = is_not_same<T, TParent>>
    operator T();
};

template<class TParent>
struct any_type_ref_fwd {
    template<class T, class = is_not_same<T, TParent>>
    operator T();

    template<class T, class = is_not_same<T, TParent>>
    operator T&() const;

    BOOST_DI_WKND(BOOST_DI_GCC)(
        template<class T, class = is_not_same<T, TParent>>
        operator T&&() const;
    )()

    template<class T, class = is_not_same<T, TParent>>
    operator const T&() const;
};

}}}} // boost::di::v1::core

#endif

