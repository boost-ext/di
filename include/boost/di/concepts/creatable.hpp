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

BOOST_DI_CFG_ERRORS_DESC_BEGIN

template<class T>
struct polymorphic_type {
struct is_not_bound {
    constexpr operator T*() const {
        return
            creatable_constraint_not_satisfied
        ();
    }

    constexpr T*
    creatable_constraint_not_satisfied(_ = "type not bound, did you forget to add: 'bind<interface, implementation>'?")
    const;
};};

template<class>
struct type_ {
    template<class T>
    constexpr operator T(){
        return {};
    }

    template<class T>
    constexpr operator T&() const{
        return
            creatable_constraint_not_satisfied_for
        <T&>();
    }

    template<class T> constexpr T
    creatable_constraint_not_satisfied_for(_ = "reference type not bound, did you forget to add `di::bind<T>.to([c]ref(value))`, notice that `di::bind<T>.to(value)` won't work!")
    const;
};

BOOST_DI_CFG_ERRORS_DESC_END

template<class T, class>
struct Any {
    using type = type_<T>;
};

template<class T, class... TCtor>
struct type {
struct is_not_bound {
    constexpr operator T*() const {
        return new T(typename Any<T, TCtor>::type{}...);
    }

    constexpr T
    creatable_constraint_not_satisfied(_ = "type not bound, did you forget to add: 'bind<interface, implementation>'?")
    const;
};

template<class X>
struct is_not_convertible_to {
    constexpr operator X() const {
        return
            creatable_constraint_not_satisfied
        ();
    }

    constexpr X
    creatable_constraint_not_satisfied(_ = "type not convertible, missing 'di::bind<type>.to(ref(value))'")
    const;
};};

template<class T>
struct number_of_constructor_arguments_doesnt_match_for {
template<int Given> struct given {
template<int Expected> struct expected {
    constexpr operator T() const {
        return
            creatable_constraint_not_satisfied
        ();
    }

    constexpr T
    creatable_constraint_not_satisfied(_ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits")
    const;
};};};

template<class>
struct allocating_an_object_of_abastract_class_type {
    struct has_unimplemented_pure_virtual_methods { };
};

namespace concepts {

template<class T, class, class = void>
struct creatable_error_impl : polymorphic_type<aux::decay_t<T>>::is_not_bound
{ };

template<class>
struct ctor_size;

template<class TInit, class... TCtor>
struct ctor_size<aux::pair<TInit, aux::type_list<TCtor...>>>
    : std::integral_constant<int, sizeof...(TCtor)>
{ };

template<class T, class... TCtor>
struct creatable_error_impl<T, aux::type_list<TCtor...>, std::enable_if_t<!std::is_polymorphic<aux::decay_t<T>>{} && ctor_size<typename type_traits::ctor<aux::decay_t<T>, type_traits::ctor_impl_t<std::is_constructible, aux::decay_t<T>>>::type>{} == sizeof...(TCtor)>>
    : type<aux::decay_t<T>, TCtor...>::is_not_bound
{ };

template<class T, class... TCtor>
struct creatable_error_impl<T, aux::type_list<TCtor...>, std::enable_if_t<ctor_size<typename type_traits::ctor<aux::decay_t<T>, type_traits::ctor_impl_t<std::is_constructible, aux::decay_t<T>>>::type>{} != sizeof...(TCtor)>>
    : number_of_constructor_arguments_doesnt_match_for<aux::decay_t<T>>
        ::template given<sizeof...(TCtor)>
        ::template expected<ctor_size<typename type_traits::ctor<aux::decay_t<T>, type_traits::ctor_impl_t<std::is_constructible, aux::decay_t<T>>>::type>{}>
{ };

template<class TInitialization, class T, class... Ts>
constexpr auto creatable() {
    return std::conditional_t<
        std::is_same<TInitialization, type_traits::uniform>{}
      , aux::is_braces_constructible<T, Ts...>
      , std::is_constructible<T, Ts...>
    >{};
}

template<class T, class... Ts>
constexpr T creatable_error() {
    return creatable_error_impl<T, aux::type_list<Ts...>>{};
}

}}} // boost::di::concepts

#endif

