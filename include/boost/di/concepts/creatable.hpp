//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_CREATABLE_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

#define BOOST_DI_CONCEPTS_CREATABLE_ATTR \
    BOOST_DI_ATTR_ERROR("creatable constraint not satisfied")

namespace boost { namespace di {

template<class T>
struct abstract_type {
struct is_not_bound {
    constexpr operator T*() const {
        using constraint_not_satisfied = is_not_bound;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr T*
    error(_ = "type not bound, did you forget to add: 'di::bind<interface, implementation>'?")
    const;
};

struct is_not_fully_implemented {
    constexpr operator T*() const {
        using constraint_not_satisfied = is_not_fully_implemented;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr T*
    error(_ = "type not implemented, did you forget to implement all interface methods?")
    const;
};

template<class TName>
struct named {
struct is_not_bound {
    constexpr operator T*() const {
        using constraint_not_satisfied = is_not_bound;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr T*
    error(_ = "type not bound, did you forget to add: 'di::bind<interface, implementation>.named(name)'?")
    const;
};

struct is_not_fully_implemented {
    constexpr operator T*() const {
        using constraint_not_satisfied = is_not_fully_implemented;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr T*
    error(_ = "type not implemented, did you forget to implement all interface methods?")
    const;
};

};};

template<class TParent>
struct when_creating {
    template<class T>
    struct type {
        template<class TName>
        struct named {
            constexpr T
            error(_ = "reference type not bound, did you forget to add `di::bind<T>.named(name).to([c]ref(value))`, notice that `di::bind<T>.named(name).to(value)` won't work!");
        };

        constexpr T
        error(_ = "reference type not bound, did you forget to add `di::bind<T>.to([c]ref(value))`, notice that `di::bind<T>.to(value)` won't work!");
    };
};

template<class TParent, class TName = no_name>
struct in_type {
    template<class T>
    using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

    template<class T, class = is_not_same<T>>
    constexpr operator T() {
        return {};
    }

    template<class T, class = is_not_same<T>>
    constexpr operator T&() const {
        using constraint_not_satisfied = typename when_creating<TParent>::template type<T&>::template named<TName>;
        return
            constraint_not_satisfied{}.error();
    }
};

template<class TParent>
struct in_type<TParent, no_name> {
    template<class T>
    using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

    template<class T, class = is_not_same<T>>
    constexpr operator T() {
        return {};
    }

    template<class T, class = is_not_same<T>>
    constexpr operator T&() const {
        using constraint_not_satisfied = typename when_creating<TParent>::template type<T&>;
        return
            constraint_not_satisfied{}.error();
    }
};

template<class...>
struct type;

template<class T, class>
struct in {
    using type = in_type<T>;
};

template<class TParent, class TName, class T>
struct in<TParent, type_traits::named<TName, T>> {
    using type = in_type<TParent, TName>;
};

template<class T, class TInitialization, class... TArgs, class... TCtor>
struct type<T, aux::pair<TInitialization, aux::type_list<TArgs...>>, TCtor...> {

template<class, class = void>
struct args;

template<class TDummy>
struct args<type_traits::direct, TDummy> {
    constexpr operator T*() const {
        return impl<T>();
    }

    template<class T_>
    auto impl() const {
        return new T{typename in<T_, TArgs>::type{}...};
    }
};

template<class TDummy>
struct args<type_traits::uniform, TDummy> {
    constexpr operator T*() const {
        return impl<T>(aux::is_braces_constructible<T, TCtor...>{});
    }

    template<class T_>
    auto impl(const std::true_type&) const {
        return new T_{typename in<T_, TArgs>::type{}...};
    }

    template<class T_>
    auto impl(const std::false_type&) const {
        return nullptr;
    }
};};

template<class T>
struct type<T> {
template<class To>
struct is_not_convertible_to {
    constexpr operator To() const {
        using constraint_not_satisfied = is_not_convertible_to;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr To
    error(_ = "wrapper is not convertible to requested type, did you mistake the scope?")
    const;
};};

template<class T>
struct number_of_constructor_arguments_doesnt_match_for {
template<int Given> struct given {
template<int Expected> struct expected {
    constexpr operator T*() const {
        using constraint_not_satisfied = expected;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr T*
    error(_ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits")
    const;
};};};

template<class T>
struct number_of_constructor_arguments_is_out_of_range_for {
template<int TMax> struct max {
    constexpr operator T*() const {
        using constraint_not_satisfied = max;
        return
            constraint_not_satisfied{}.error();
    }

    constexpr T*
    error(_ = "increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters")
    const;
};};

namespace concepts {

template<class>
struct ctor_size;

template<class TInit, class... TCtor>
struct ctor_size<aux::pair<TInit, aux::type_list<TCtor...>>>
    : std::integral_constant<int, sizeof...(TCtor)>
{ };

template<class...>
struct creatable_error_impl;

template<class T>
using ctor_size_t = ctor_size<
    typename type_traits::ctor<
        aux::decay_t<T>
      , type_traits::ctor_impl_t<std::is_constructible, aux::decay_t<T>>
    >::type
>;

#define BOOST_DI_NAMED_ERROR(name, type, error_type, error) \
    std::conditional_t< \
        std::is_same<TName, no_name>::value \
      , typename error_type<aux::decay_t<type>>::error \
      , typename error_type<aux::decay_t<type>>::template named<TName>::error \
    >

template<class TInitialization, class TName, class I, class T, class... TCtor>
struct creatable_error_impl<TInitialization, TName, I, T, aux::type_list<TCtor...>>
    : std::conditional_t<
          std::is_abstract<aux::decay_t<T>>::value
        , std::conditional_t<
              std::is_same<I, T>::value
            , BOOST_DI_NAMED_ERROR(TName, T, abstract_type, is_not_bound)
            , BOOST_DI_NAMED_ERROR(TName, T, abstract_type, is_not_fully_implemented)
          >
        , std::conditional_t<
              ctor_size_t<T>::value == sizeof...(TCtor)
            , std::conditional_t<
                  !sizeof...(TCtor)
                , typename number_of_constructor_arguments_is_out_of_range_for<aux::decay_t<T>>::
                      template max<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
                , typename type<
                      aux::decay_t<T>
                    , typename type_traits::ctor_traits<aux::decay_t<T>>::type, TCtor...
                  >::template args<TInitialization>
              >
            , typename number_of_constructor_arguments_doesnt_match_for<aux::decay_t<T>>::
                  template given<sizeof...(TCtor)>::
                  template expected<ctor_size_t<T>::value>
          >
      >
{ };

#undef BOOST_DI_NAMED_ERROR

template<class TInit, class T, class... Ts>
struct creatable {
    static constexpr auto value = std::is_constructible<T, Ts...>::value;
};

template<class T, class... Ts>
struct creatable<type_traits::uniform, T, Ts...> {
    static constexpr auto value = aux::is_braces_constructible<T, Ts...>::value;
};

template<class TInitialization, class TName, class I, class T, class... Ts>
constexpr T creatable_error() {
    return creatable_error_impl<TInitialization, TName, I, T, aux::type_list<Ts...>>{};
}

}}} // boost::di::concepts

#endif

