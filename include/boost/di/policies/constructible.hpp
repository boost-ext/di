//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOS_DI_POLICIES_CONSTRUCTIBLE_HPP
#define BOOS_DI_POLICIES_CONSTRUCTIBLE_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { namespace policies {

template<class T>
struct type {
template<class TPolicy>
struct not_allowed_by {
    operator T() const {
        using constraint_not_satisfied = not_allowed_by;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr T
    error(_ = "type disabled by constructible policy, added by BOOST_DI_CFG or make_injector<CONFIG>!")
    const;
};};

struct _ { };
struct type_op {};

template<class T, class = void>
struct apply_impl {
    template<class TArg>
    static auto apply(const TArg&) noexcept {
        return T{};
    }
};

template<template<class...> class T, class... Ts>
struct apply_impl<T<Ts...>, std::enable_if_t<!std::is_base_of<type_op, T<Ts...>>::value>> {
    template<class TOp, class>
    struct apply_placeholder_impl {
        using type = TOp;
    };

    template<class TOp>
    struct apply_placeholder_impl<_, TOp> {
        using type = TOp;
    };

    template<template<class...> class TExpr, class TOp, class... TArgs>
    struct apply_placeholder {
        using type = TExpr<typename apply_placeholder_impl<TArgs, TOp>::type...>;
    };

    template<class TArg>
    static auto apply(const TArg&) noexcept {
        using type = typename TArg::type;
        return typename apply_placeholder<T, type, Ts...>::type{};
    }
};

template<class T>
struct apply_impl<T, std::enable_if_t<std::is_base_of<type_op, T>::value>> {
    template<class TArg>
    static auto apply(const TArg& data) noexcept {
        return T::apply(data);
    }
};

template<class T>
struct not_ : type_op {
	template<class TArg>
    static auto apply(const TArg& data) noexcept {
        return std::integral_constant<bool
          , !decltype(apply_impl<T>::apply(data))::value
        >{};
    }
};

template<class... Ts>
struct and_ : type_op {
	template<class TArg>
    static auto apply(const TArg& data) noexcept {
        return std::is_same<
            aux::bool_list<decltype(apply_impl<Ts>::apply(data))::value...>
          , aux::bool_list<(decltype(apply_impl<Ts>::apply(data)){}, true)...>
        >{};
    }
};

template<class... Ts>
struct or_ : type_op {
	template<class TArg>
    static auto apply(const TArg& data) noexcept {
        return std::integral_constant<bool
          , !std::is_same<
                aux::bool_list<decltype(apply_impl<Ts>::apply(data))::value...>
              , aux::bool_list<(decltype(apply_impl<Ts>::apply(data)){}, false)...>
            >::value
        >{};
    }
};

template<class T>
struct always : type_op {
	template<class TArg>
	static auto apply(const TArg& data) noexcept {
		return apply_impl<T>::apply(data);
	}
};

template<class T>
struct is_bound : type_op {
	template<class TArg>
	static auto apply(const TArg&) noexcept {
        struct not_resolved { };
        using type = std::conditional_t<std::is_same<T, _>::value, typename TArg::type, T>;
        using dependency = typename TArg::template resolve<type, typename TArg::name, not_resolved>;
        return std::integral_constant<bool, !std::is_same<dependency, not_resolved>::value>{};
    }
};

struct is_root : type_op {
	template<class TArg>
	static auto apply(const TArg&) noexcept {
        return typename TArg::is_root{};
    }
};

namespace operators {

template<class X, class Y>
inline auto operator||(const X&, const Y&) {
    return or_<X, Y>{};
}

template<class X, class Y>
inline auto operator&&(const X&, const Y&) {
    return and_<X, Y>{};
}

template<class T>
inline auto operator!(const T&) {
    return not_<T>{};
}

} // operators

template<class T>
struct constructible_impl {
    template<class TArg, std::enable_if_t<decltype(T::apply(TArg{}))::value, int> = 0>
    std::true_type operator()(const TArg& data) const {
        T::apply(data);
        return {};
    }

    template<class TArg, std::enable_if_t<!decltype(T::apply(TArg{}))::value, int> = 0>
    std::false_type operator()(const TArg&) const {
        void(static_cast<typename TArg::type>(typename type<typename TArg::type>::template not_allowed_by<T>{}));
        return {};
    }
};

template<class T = aux::never<_>, std::enable_if_t<std::is_base_of<type_op, T>::value, int> = 0>
inline auto constructible(const T& = {}) {
	return constructible_impl<T>{};
}

template<class T = aux::never<_>, std::enable_if_t<!std::is_base_of<type_op, T>::value, int> = 0>
inline auto constructible(const T& = {}) {
	return constructible_impl<or_<T>>{};
}

}}} // boost::di::policies

#endif

