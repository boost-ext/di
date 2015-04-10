//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/core/injector.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di { namespace detail {

#if !defined(_MSC_VER)
template<class>
void create(const std::true_type&) { }

template<class>
BOOST_DI_CONCEPTS_CREATABLE_ATTR
void
    create
(const std::false_type&) { }

template<class...>
struct is_creatable
    : std::true_type
{ };

template<class TInjector, class T>
struct is_creatable<std::true_type, TInjector, T>
    : std::integral_constant<bool
        , TInjector::template is_creatable<T>() ||
          TInjector::template is_creatable<T*>()
      >
{ };
#endif

} // namespace detail

template<class... T>
class injector : public
#if !defined(_MSC_VER)
     BOOST_DI_REQUIRES_MSG_T(concepts::boundable<aux::type<T...>>
#else                          , core::injector<::BOOST_DI_CFG, T...>) {
	core::injector<::BOOST_DI_CFG, T...> {
#endif
public:
    template<
        class TConfig
      , class... TArgs
#if !defined(_MSC_VER)
#if !defined(__clang__)
     , BOOST_DI_REQUIRES_MSG(concepts::boundable<aux::type<T...>>)
#endif
#endif
    > injector(const core::injector<TConfig, TArgs...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, T...>(injector) {
#if !defined(_MSC_VER)
        using namespace detail;
        int _[]{0, (
            create<T>(
                detail::is_creatable<
                    typename std::is_same<concepts::configurable<TConfig>, std::true_type>::type
                  , core::injector<TConfig, TArgs...>
                  , T
                >{}
            )
        , 0)...}; (void)_;
#endif
    }
};

}} // boost::di

#endif

