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
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "type not bound, did you forget to add: 'di::bind<interface, implementation>'?")
    const;
};

template<class TName>
struct named {
struct is_not_bound {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "type not bound, did you forget to add: 'di::bind<interface, implementation>.named(name)'?")
    const;
};};};

template<class T, class TParent>
using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

template<class TParent>
struct type_ {
    template<class T, class = is_not_same<T, TParent>>
    constexpr operator T(){
        return {};
    }

    template<class T, class = is_not_same<T, TParent>>
    constexpr operator T&() const{
        return
            constraint_not_satisfied_for
        <T&>();
    }

    template<class T> constexpr T
    constraint_not_satisfied_for(_ = "reference type not bound, did you forget to add `di::bind<T>.to([c]ref(value))`, notice that `di::bind<T>.to(value)` won't work!")
    const;
};

template<class T, class>
struct Any {
    using type = type_<T>;
};

template<class T, class... TCtor>
struct type {
struct is_not_creatable {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "type is not creatable'?")
    const;
};

template<class, class = void>
struct args;

template<class TDummy>
struct args<type_traits::direct, TDummy> {
    constexpr operator T*() const {
        return impl<T>();
    }

    template<class T_>
    T_* impl() const {
        return new T{typename Any<T_, TCtor>::type{}...};
    }
};

template<class TDummy>
struct args<type_traits::uniform, TDummy> {
    constexpr operator T*() const {
        return impl<T>();
    }

    template<class T_, std::enable_if_t<aux::is_braces_constructible<T_, TCtor...>{}, int> = 0>
    T_* impl() const {
        return new T_{typename Any<T_, TCtor>::type{}...};
    }

    template<class T_, std::enable_if_t<!aux::is_braces_constructible<T_, TCtor...>{}, int> = 0>
    T_* impl() const {
        return nullptr;
    }
};

template<class To>
struct is_not_convertible_to {
    constexpr operator To() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr To
    constraint_not_satisfied(_ = "type not convertible, missing 'di::bind<type>.to(ref(value))'")
    const;
};};

template<class T>
struct number_of_constructor_arguments_doesnt_match_for {
template<int Given> struct given {
template<int Expected> struct expected {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits")
    const;
};};};

template<class T>
struct number_of_constructor_arguments_is_out_of_range_for {
template<int TMax> struct max {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters")
    const;
};};

template<class>
struct allocating_an_object_of_abastract_class_type {
    struct has_unimplemented_pure_virtual_methods { };
};

BOOST_DI_CFG_ERRORS_DESC_END

namespace concepts {

template<class>
struct ctor_size;

template<class TInit, class... TCtor>
struct ctor_size<aux::pair<TInit, aux::type_list<TCtor...>>>
    : std::integral_constant<int, sizeof...(TCtor)>
{ };

template<class, class, class, class>
struct creatable_error_impl;

template<class T>
using ctor_size_t = ctor_size<
    typename type_traits::ctor<
        aux::decay_t<T>
      , type_traits::ctor_impl_t<std::is_constructible, aux::decay_t<T>>
    >::type
>;

template<class TInitialization, class TName, class T, class... TCtor>
struct creatable_error_impl<TInitialization, TName, T, aux::type_list<TCtor...>>
    : std::conditional_t<
          std::is_polymorphic<aux::decay_t<T>>{}
        , std::conditional_t<std::is_same<TName, no_name>{}, typename polymorphic_type<aux::decay_t<T>>::is_not_bound, typename polymorphic_type<aux::decay_t<T>>::template named<TName>::is_not_bound>
        , std::conditional_t<
              ctor_size_t<T>{} == sizeof...(TCtor)
            , std::conditional_t<
                  !sizeof...(TCtor)
                , typename number_of_constructor_arguments_is_out_of_range_for<aux::decay_t<T>>::
                      template max<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
                , typename type<aux::decay_t<T>, TCtor...>::template args<TInitialization>
              >
            , typename number_of_constructor_arguments_doesnt_match_for<aux::decay_t<T>>::
                  template given<sizeof...(TCtor)>::
                  template expected<ctor_size_t<T>{}>
          >
      >
{ };

template<class TInit, class T, class... Ts>
struct creatable {
    static constexpr auto value = std::is_constructible<T, Ts...>::value;
};

template<class T, class... Ts>
struct creatable<type_traits::uniform, T, Ts...> {
    static constexpr auto value = aux::is_braces_constructible<T, Ts...>::value;
};

template<class TInitialization, class TName, class T, class... Ts>
constexpr T creatable_error() {
    return creatable_error_impl<TInitialization, TName, T, aux::type_list<Ts...>>{};
}

auto creatable_impl_(...) -> std::false_type;

template<class T, class B, class N, class TIsRoot>
auto creatable_impl_(T&& t, B&&, N&&, TIsRoot&&) -> aux::is_valid_expr<decltype(t.template create_impl_<B, N, TIsRoot>())>;

template<class T, class B, class N = no_name, class TIsRoot = std::false_type>
constexpr auto creatable_() {
    return decltype(creatable_impl_(std::declval<T>(), std::declval<B>(), std::declval<N>(), std::declval<TIsRoot>())){};
}

}}} // boost::di::concepts

#endif

