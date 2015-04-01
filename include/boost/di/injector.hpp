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
#include "boost/di/config.hpp"

namespace boost { namespace di {

template<class>
void create(const std::true_type&) { }

template<class>
BOOST_DI_ATTR_ERROR("creatable constraint not satisfied")
void
    create
(const std::false_type&) { }

template<class... T>
class injector : public
     BOOST_DI_REQUIRES_ERR_T(concepts::boundable<aux::type<T...>>
                           , core::injector<::BOOST_DI_CFG, T...>) {
public:
    template<
        class TConfig
      , class... TArgs
#if !defined(__clang__)
     , BOOST_DI_REQUIRES_ERR(concepts::boundable<aux::type<T...>>)
#endif
    > injector(const core::injector<TConfig, TArgs...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, T...>{injector} {
        int _[]{0, (
            create<T>(
                std::integral_constant<bool
                    , concepts::creatable_<core::injector<TConfig, TArgs...>, T>() ||
                      concepts::creatable_<core::injector<TConfig, TArgs...>, T*>()
                >{}
            )
        , 0)...}; (void)_;
    }
};

}} // boost::di

#endif

