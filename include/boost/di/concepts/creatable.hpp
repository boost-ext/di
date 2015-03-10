//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_CREATABLE_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost { namespace di {

#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wundefined-inline"

struct $ { $(...) { } };

template<class T>
struct polymorphic_type {
    struct is_not_bound {
        constexpr operator T() const {
            return error();
        }
        constexpr T error($ = "type not bound, did you forget to add: 'bind<interface, implementation>'?") const;
    };
};

template<class>
struct type_ {
    template<class T>
    constexpr operator T(){
        return {};
    };

    template<class T>
    constexpr operator T&() const{
        return error<T&>();
    };

    template<class T>
    constexpr T error($ = "reference type not bound, did you forget to add `di::bind<T>.to([c]ref(value))`, notice that `di::bind<T>.to(value)` won't work!") const;
};

template<class T, class>
struct Any {
    using type = type_<T>;
};

template<class T, class... TCtor>
struct type {
    struct is_not_bound {
        constexpr operator T() const {
            return T(typename Any<T, TCtor>::type{}...);
        }
        constexpr T error($ = "type not bound, did you forget to add: 'bind<interface, implementation>'?") const;
    };

    template<class X>
    struct is_not_convertible_to {
        constexpr operator X() const {
            return error();
        }
        constexpr X error($ = "type not convertible, missing 'di::bind<type>.to(ref(value))'") const;
    };
};

template<class T>
struct number_of_constructor_arguments_doesnt_match_for {
    template<int Given>
    struct given {
        template<int Expected>
        struct expected {
            constexpr operator T() const {
                return error();
            }
            constexpr T error($ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits");
        };
    };
};

template<class>
struct allocating_an_object_of_abastract_class_type {
    struct has_unimplemented_pure_virtual_methods { };
};

namespace concepts {

template<class T, class, class = void>
struct creatable_impl : polymorphic_type<T>::is_not_bound
{ };

template<class T, class... TCtor>
struct creatable_impl<T, aux::type_list<TCtor...>, std::enable_if_t<!std::is_polymorphic<aux::decay_t<T>>{}>>
    : type<T, TCtor...>::is_not_bound
{ };

template<class>
struct ctor_size;

template<class TInit, class... TCtor>
struct ctor_size<aux::pair<TInit, aux::type_list<TCtor...>>>
    : std::integral_constant<int, sizeof...(TCtor)>
{ };

template<class T, class... TCtor>
struct creatable_impl<T, aux::type_list<TCtor...>, std::enable_if_t<ctor_size<typename type_traits::ctor<T, type_traits::ctor_impl_t<std::is_constructible, T>>::type>{} != sizeof...(TCtor)>>
    : number_of_constructor_arguments_doesnt_match_for<T>
        ::template given<sizeof...(TCtor)>
        ::template expected<ctor_size<typename type_traits::ctor<T, type_traits::ctor_impl_t<std::is_constructible, T>>::type>{}>
{ };

template<class T, class... Ts>
constexpr T creatable() {
    return creatable_impl<T, aux::type_list<Ts...>>{};
}

template<class, class T, class... Ts>
T creatable2(std::enable_if_t<std::is_constructible<T, Ts...>{}>* = 0);

template<class, class T, class... Ts>
constexpr auto creatable2(std::enable_if_t<!std::is_constructible<T, Ts...>{}>* = 0) {
    return creatable_impl<T, aux::type_list<Ts...>>{};
}

#pragma GCC diagnostic pop

}}} // boost::di::concepts

#endif

