//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/core/injector.hpp"

namespace boost {
namespace di {

namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

template<typename T>
using is_injector = bool_<has_deps<T>::value || aux::has_configure<T>::value>;

template<typename T, typename = void>
struct get_injector {
    using type = T;
};

template<typename T>
struct get_injector<T, aux::enable_if_t<aux::has_configure<T>{}>> {
    using type = typename aux::function_traits<decltype(&T::configure)>::result_type;
};

template<typename T, typename = void>
struct get_deps {
    using type = typename T::deps;
};

template<typename T>
struct get_deps<T, aux::enable_if_t<aux::has_configure<T>{}>> {
    using type = typename get_injector<T>::type::deps;
};

template<typename T, typename = typename is_type_list<T>::type, typename = typename is_injector<T>::type>
struct add_type_list;

template<typename T, typename TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = T;
};

template<typename T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = typename get_deps<T>::type;
};

template<typename T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = type_list<T>; //scope<scopes::deduce>::bind<T>;
};

template<typename... Ts>
using bindings_t = typename join<typename add_type_list<Ts>::type...>::type;

} // namespace detail

template<typename... Ts>
class injector
    : public core::injector<detail::bindings_t<Ts...>>
{
public:
    template<typename... TArgs>
    injector(const TArgs&... args)
        : core::injector<detail::bindings_t<Ts...>>(pass_arg(args)...)
    { }

private:
    template<typename T>
    aux::enable_if_t<!aux::has_configure<T>::value, const T&>
    pass_arg(const T& arg) const {
        return arg;
    }

    template<typename T>
    aux::enable_if_t<aux::has_configure<T>::value, typename detail::get_injector<T>::type>
    pass_arg(const T& arg) const {
        return arg.configure();
    }
};

} // namespace di
} // namespace boost

#endif

