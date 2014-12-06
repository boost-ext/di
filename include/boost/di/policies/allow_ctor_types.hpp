//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOS_DI_POLICIES_ALLOW_TYPES_HPP
#define BOOS_DI_POLICIES_ALLOW_TYPES_HPP

#include <type_traits>

namespace boost { namespace di { namespace policies {

struct _ { };
struct type_op {};

template<class T, class = void>
struct apply_impl {
    template<class TData>
    static auto apply(const TData&) noexcept {
        return T{};
    }
};

template<template<class...> class T, class... Ts>
struct apply_impl<T<Ts...>, std::enable_if_t<!std::is_base_of<type_op, T<Ts...>>{}>> {
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

    template<class TData>
    static auto apply(const TData& data) noexcept {
        using type = typename TData::type;
        return typename apply_placeholder<T, type, Ts...>::type{};
    }
};

template<class T>
struct apply_impl<T, std::enable_if_t<std::is_base_of<type_op, T>{}>> {
    template<class TData>
    static auto apply(const TData& data) noexcept {
        return T::apply(data);
    }
};

template<bool...>
struct bool_seq { };

template<class T>
struct not_ : type_op {
	template<class TData>
    static auto apply(const TData& data) noexcept {
        return std::integral_constant<bool
          , !decltype(apply_impl<T>::apply(data)){}
        >{};
    }
};

template<class... Ts>
struct and_ : type_op {
	template<class TData>
    static auto apply(const TData& data) noexcept {
        return std::is_same<
            bool_seq<decltype(apply_impl<Ts>::apply(data)){}...>
          , bool_seq<(decltype(apply_impl<Ts>::apply(data)){}, true)...>
        >{};
    }
};

template<class... Ts>
struct or_ : type_op {
	template<class TData>
    static auto apply(const TData& data) noexcept {
        return std::integral_constant<bool
          , !std::is_same<
                bool_seq<decltype(apply_impl<Ts>::apply(data)){}...>
              , bool_seq<(decltype(apply_impl<Ts>::apply(data)){}, false)...>
            >{}
        >{};
    }
};

template<class T>
struct always : type_op {
	template<class TData>
	static auto apply(const TData& data) noexcept {
		return apply_impl<T>::apply(data);
	}
};

template<class T>
struct is_bound : type_op {
	template<class TData>
	static auto apply(const TData& data) noexcept {
        struct not_resolved { };
        using type = std::conditional_t<std::is_same<T, _>{}, typename TData::type, T>;
        auto dep = data.template resolve<type, not_resolved>();
        return std::integral_constant<bool
          , !std::is_same<decltype(dep), not_resolved>{}
        >{};
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
struct allow_ctor_types_impl {
    template<class TData>
    void operator()(const TData& data) const noexcept {
		static_assert(decltype(T::apply(data)){}, "Type T is not allowed");
    }
};

template<class T = std::false_type>
inline auto allow_ctor_types(const T& = {}) noexcept {
	return allow_ctor_types_impl<always<T>>{};
};

}}} // boost::di::policies

#endif

