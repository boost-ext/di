//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_CREATABLE_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

#define BOOST_DI_CONCEPTS_CREATABLE_ERROR_MSG \
    BOOST_DI_DEPRECATED("creatable constraint not satisfied")

namespace boost { namespace di { inline namespace v1 { namespace concepts {

template<class T>
struct abstract_type {
struct is_not_bound {
    operator T*() const {
        using constraint_not_satisfied = is_not_bound;
        return
            constraint_not_satisfied{}.error();
    }

    static inline T*
    error(_ = "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?");
};

struct is_not_fully_implemented {
    operator T*() const {
        using constraint_not_satisfied = is_not_fully_implemented;
        return
            constraint_not_satisfied{}.error();
    }

    static inline T*
    error(_ = "type is not implemented, did you forget to implement all interface methods?");
};

template<class TName>
struct named {
struct is_not_bound {
    operator T*() const {
        using constraint_not_satisfied = is_not_bound;
        return
            constraint_not_satisfied{}.error();
    }

    static inline T*
    error(_ = "type is not bound, did you forget to add: 'di::bind<interface>.named(name).to<implementation>()'?");
};

struct is_not_fully_implemented {
    operator T*() const {
        using constraint_not_satisfied = is_not_fully_implemented;
        return
            constraint_not_satisfied{}.error();
    }

    static inline T*
    error(_ = "type is not implemented, did you forget to implement all interface methods?");
};
};};

template<class T>
struct type {

template<class To>
struct is_not_convertible_to {
    operator To() const {
        using constraint_not_satisfied = is_not_convertible_to;
        return
            constraint_not_satisfied{}.error();
    }

    static inline To
    error(_ = "wrapper is not convertible to requested type, did you mistake the scope?");
};

struct has_ambiguous_number_of_constructor_parameters {
template<int Given> struct given {
template<int Expected> struct expected {
    operator T*() const {
        using constraint_not_satisfied = expected;
        return
            constraint_not_satisfied{}.error();
    }

    static inline T*
    error(_ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits");
};};};

struct has_to_many_constructor_parameters {
template<int TMax> struct max {
    operator T*() const {
        using constraint_not_satisfied = max;
        return
            constraint_not_satisfied{}.error();
    }

    static inline T*
    error(_ = "increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters");
};};

};

template<class>
struct ctor_size;

template<class TInit, class... TCtor>
struct ctor_size<aux::pair<TInit, aux::type_list<TCtor...>>>
    : aux::integral_constant<int, sizeof...(TCtor)>
{ };

template<class...>
struct creatable_error_impl;

template<class T>
using ctor_size_t = ctor_size<
    typename type_traits::ctor<
        aux::decay_t<T>
      , type_traits::ctor_impl_t<aux::is_constructible, aux::decay_t<T>>
    >::type
>;

#define BOOST_DI_NAMED_ERROR(name, type, error_type, error) \
    aux::conditional_t< \
        aux::is_same<TName, no_name>::value \
      , typename error_type<aux::decay_t<type>>::error \
      , typename error_type<aux::decay_t<type>>::template named<TName>::error \
    >

template<class TInitialization, class TName, class I, class T, class... TCtor>
struct creatable_error_impl<TInitialization, TName, I, T, aux::type_list<TCtor...>>
    : aux::conditional_t<
          aux::is_abstract<aux::decay_t<T>>::value
        , aux::conditional_t<
              aux::is_same<I, T>::value
            , BOOST_DI_NAMED_ERROR(TName, T, abstract_type, is_not_bound)
            , BOOST_DI_NAMED_ERROR(TName, T, abstract_type, is_not_fully_implemented)
          >
        , aux::conditional_t<
              ctor_size_t<T>::value == sizeof...(TCtor)
            , typename type<aux::decay_t<T>>::has_to_many_constructor_parameters::
                  template max<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
            , typename type<aux::decay_t<T>>::has_ambiguous_number_of_constructor_parameters::
                  template given<sizeof...(TCtor)>::
                  template expected<ctor_size_t<T>::value>
          >
      >
{ };

#undef BOOST_DI_NAMED_ERROR

template<class TInit, class T, class... TArgs>
struct creatable {
    static constexpr auto value = aux::is_constructible<T, TArgs...>::value;
};

template<class T, class... TArgs>
struct creatable<type_traits::uniform, T, TArgs...> {
    static constexpr auto value = aux::is_braces_constructible<T, TArgs...>::value;
};

template<class TInitialization, class TName, class I, class T, class... TArgs>
T creatable_error() {
    return creatable_error_impl<TInitialization, TName, I, T, aux::type_list<TArgs...>>{};
}

}}}} // boost::di::v1::concepts

#endif

