//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOS_DI_POLICIES_ALLOW_TYPES_HPP
#define BOOS_DI_POLICIES_ALLOW_TYPES_HPP

namespace boost { namespace di { inline namespace policies {

struct _ { };
struct type_op {};

template<class T, class>
struct apply_impl {
	using type = T;
};

template<class T>
struct apply_impl<_, T> {
	using type = T;
};

template<template<class...> class X, class T, class... Ts>
struct apply_ {
	using type = X<typename apply_impl<Ts, T>::type...>;
};

template<class, class T, class = void>
struct apply {
    using type = T;
};

template<class X, template<class...> class T, class... Ts>
struct apply<X, T<Ts...>, std::enable_if_t<!std::is_base_of<type_op, T<Ts...>>{}>> {
	using type = typename apply_<T, X, Ts...>::type;
};

template<class X, class T>
struct apply<X, T, std::enable_if_t<std::is_base_of<type_op, T>{}>> {
	using type = decltype(T::apply(X{}));
};

template<class T, class X>
using apply_t = typename apply<T, X>::type;

template<bool...>
struct bool_seq { };

template<typename X>
struct not_ : type_op {
	template<class T>
	static auto apply(const T&) noexcept {
		return std::integral_constant<bool, !apply_t<T, X>{}>{};
	}
};

template<typename... Ts>
struct and_ : type_op {
	template<class T>
	static auto apply(const T&) noexcept {
		return std::is_same<bool_seq<apply_t<T, Ts>{}...>, bool_seq<(apply_t<T, Ts>{}, true)...>>{};
	}
};

template<typename... Ts>
struct or_ : type_op {
	template<class T>
	static auto apply(const T&) noexcept {
		return std::integral_constant<bool, !std::is_same<bool_seq<apply_t<T, Ts>{}...>, bool_seq<(apply_t<T, Ts>{}, false)...>>{}>{};
	}
};

template<class X>
struct always : type_op {
	template<class T>
	static auto apply(const T&) noexcept {
		return apply_t<T, X>{};
	}
};

struct is_bound : type_op {
	template<class TData>
	static auto apply(const TData& data) noexcept {
        struct not_resolved { };
        using type = typename TData::type;
        auto dep = data.template resolve<type, not_resolved>();
        return std::is_same<decltype(dep), not_resolved>{};
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

template<class X>
inline auto operator!(const X&) {
	return not_<X>{};
}

} // namespace operators

template<typename T>
struct allow_ctor_types_impl {
    template<class TData>
    void operator()(const TData& data) const noexcept {
        using type = typename TData::type;
		static_assert(decltype(T::apply(type{})){}, "Type T is not allowed");
    }
};

template<class T = std::false_type>
inline auto allow_ctor_types(const T& = {}) noexcept {
	return allow_ctor_types_impl<always<T>>{};
};

}}} // namespace boost::di::policies

#endif

