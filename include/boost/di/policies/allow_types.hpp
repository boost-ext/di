//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOS_DI_POLICIES_ALLOW_TYPES_HPP
#define BOOS_DI_POLICIES_ALLOW_TYPES_HPP

#include "boost/di/aux_/mpl.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"
#include "boost/di/scopes/deduce.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_DI_HAS_MEMBER_TYPE(element_type);
BOOST_DI_HAS_MEMBER_TYPE(value_type);

template<typename... Ts>
struct and_ {
	template<typename T>
	using allow = std::is_same<
		bool_seq<Ts::template allow<T>::value...>,
		bool_seq<(Ts::template allow<T>::value, true)...>
	>;
};

template<typename... Ts>
struct or_ {
	template<typename T>
	using allow = bool_<
        !std::is_same<
            bool_seq<Ts::template allow<T>::value...>,
            bool_seq<(Ts::template allow<T>::value, false)...>
        >::value
    >;
};

template<typename X>
struct not_ {
	template<typename T>
	using allow = bool_<!X::template allow<T>::value>;
};

template<typename X, typename Y>
and_<X, Y> operator&&(const X&, const Y&) {
    return and_<X, Y>();
}

template<typename X, typename Y>
or_<X, Y> operator||(const X&, const Y&) {
    return or_<X, Y>();
}

template<typename T>
not_<T> operator!(const T&) {
    return not_<T>();
}

struct is_smart_ptr {
    template<typename _, typename T = typename _::type>
    using allow = has_element_type<typename type_traits::remove_accessors<T>::type>;
};

struct is_ref {
    template<typename _, typename T = typename _::type>
    using allow = bool_<
        std::is_reference<T>::value &&
        !std::is_const<typename std::remove_reference<T>::type>::value &&
        !has_element_type<typename type_traits::remove_accessors<T>::type>::value
    >;
};

struct is_const_ref {
    template<typename _, typename T = typename _::type>
    using allow = bool_<
        std::is_const<typename std::remove_reference<T>::type>::value &&
        std::is_reference<T>::value &&
        !has_element_type<typename type_traits::remove_accessors<T>::type>::value
    >;
};

struct is_rvalue {
    template<typename _, typename T = typename _::type>
    using allow = std::is_rvalue_reference<T>;
};

struct is_ptr {
    template<typename _, typename T = typename _::type>
    using allow = std::is_pointer<T>;
};

struct is_copy {
    template<typename _, typename T = typename _::type>
    using allow = bool_<
        !std::is_reference<T>::value &&
        !std::is_pointer<T>::value &&
        !std::is_rvalue_reference<T>::value &&
        !has_element_type<typename type_traits::remove_accessors<T>::type>::value
    >;
};

template<typename TValueType>
struct is_type {
    template<typename _, typename T = typename _::type>
    using allow = std::is_same<T, TValueType>;
};

template<typename TExpr>
struct is_type_expr {
    //template<typename _, typename T = typename _::type>
    //using allow = ::mpl::apply<TExpr, T>::type;
};

template<typename TScope>
struct is_scope {
    template<typename _, typename T = typename _::type>
    using allow = std::is_same<T, TScope>;
};

template<>
struct is_scope<scopes::deduce>; // disabled

class is_bound {
    struct not_resolved {
        using type = not_resolved;

        template<typename>
        struct rebind {
            using other = not_resolved;
        };
    };

    template<typename T>
    using is_resolvable = bool_<
        !std::is_same<
            typename T::binder::template resolve<
                typename T::type
              , type_list<>
              , not_resolved
            >::type
          , not_resolved
        >::value
    >;

public:
    template<typename T>
    using allow = is_resolvable<T>;
};

template<typename TExpr>
class allow_types_impl {
    template<typename T>
    using assert_types = typename TExpr::template allow<T>;

public:
    template<typename TData>
    void assert_policy() const {
        static_assert(assert_types<TData>::value, "Type is not allowed");
    }
};

template<typename T>
inline allow_types_impl<T> allow_types(const T&) {
    return allow_types_impl<T>();
}

} // namespace policies
} // namespace di
} // namespace boost

#endif

