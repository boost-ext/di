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
#include "boost/di/concepts/creatable.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_TYPE(is_ref);

template<class TParent, class TInjector, class TError>
struct any_type {
    template<class T>
    struct is_ref_impl {
        static constexpr auto value =
            std::is_same<TInjector, aux::none_t>::value ||
            has_is_ref<
                std::remove_reference_t<decltype(binder::resolve<T>((TInjector*)nullptr))>
            >::value;
    };

    template<class T>
    using is_ref = std::enable_if_t<is_ref_impl<T>::value>;

    template<class T>
    using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

    template<class T, class TError_>
    struct is_creatable_impl {
        static constexpr auto value =
            std::is_same<TInjector, aux::none_t>::value || TInjector::template is_creatable<T>();
    };

    template<class T>
    struct is_creatable_impl<T, std::false_type> {
        static constexpr auto value = true;
    };

    template<class T, class TError_>
    using is_creatable = std::enable_if_t<is_creatable_impl<T, TError_>::value>;

    template<class T, class = is_not_same<T>, class = is_creatable<T, TError>>
    operator T() {
        return injector_.template create_impl<T>();
    }

    template<class T, class = is_not_same<T>, class = is_ref<T>, class = is_creatable<T&, TError>>
    operator T&() const {
        return injector_.template create_impl<T&>();
    }

#if !defined(__clang__) && !defined(_MSC_VER)
    template<class T, class = is_not_same<T>, class = is_ref<T>, class = is_creatable<T&&, TError>>
    operator T&&() const {
        return injector_.template create_impl<T&&>();
    }
#endif

    template<class T, class = is_not_same<T>, class = is_ref<T>, class = is_creatable<const T&, TError>>
    operator const T&() const {
        return injector_.template create_impl<const T&>();
    }

    const TInjector& injector_;
};

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

}}} // boost::di::core

#endif

