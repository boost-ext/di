//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/core/module.hpp"
#include "boost/di/core/pool.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/bindings.hpp"
#include "boost/di/type_traits/function_traits.hpp"

namespace boost {
namespace di {

BOOST_DI_HAS_MEMBER_TYPE(deps);
BOOST_DI_HAS_MEMBER_FUNCTION(configure, configure);

template<typename T>
struct is_module
    : bool_<has_deps<T>::value || has_configure<T>::value>
{ };

template<typename T, typename = void>
struct get_module {
    using type = T;
};

template<typename T>
struct get_module<T, typename std::enable_if<has_configure<T>::value>::type> {
    using type = typename type_traits::function_traits<
        decltype(&T::configure)
    >::result_type;
};

template<typename T, typename = void>
struct get_deps {
    using type = typename T::deps;
};

template<typename T>
struct get_deps<T, typename std::enable_if<has_configure<T>::value>::type> {
    using type = typename get_module<T>::type::deps;
};

template<typename T, typename = typename is_type_list<T>::type, typename = typename is_module<T>::type>
struct add_type_list;

template<typename T, typename Any>
struct add_type_list<T, std::true_type, Any> {
    using type = T;
};

template<typename T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = typename get_deps<T>::type;
};

template<typename T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = scope<scopes::deduce>::bind<T>;
};

template<typename... Ts>
using get_bindings = typename sort<typename join<typename add_type_list<Ts>::type...>::type>::type;

template<typename... Ts>
class injector
    : public core::module<typename get_bindings<Ts...>::type>
{
public:
    template<typename... TArgs>
    explicit injector(const TArgs&... args)
        : core::module<typename get_bindings<Ts...>::type>(pass_arg(args)...)
    { }

private:
    template<typename T>
    typename std::enable_if<!has_configure<T>::value, const T&>::type
    pass_arg(const T& arg) const {
        return arg;
    }

    template<typename T>
    typename std::enable_if<has_configure<T>::value, typename get_module<T>::type>::type
    pass_arg(const T& arg) const {
        return arg.configure();
    }
};

} // namespace di
} // namespace boost

#endif

