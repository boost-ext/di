//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/aux_/compiler_specific.hpp"
#include "boost/di/core/injector.hpp"
#include "boost/di/concepts/boundable.hpp"
#include "boost/di/concepts/creatable.hpp"
#include "boost/di/concepts/configurable.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di { inline namespace v1 { namespace detail {

template<class>
void create(const std::true_type&) { }

template<class>
BOOST_DI_CONCEPTS_CREATABLE_ATTR
void
    create
(const std::false_type&) { }

} // namespace detail

template<class... T>
class injector : public
     BOOST_DI_REQUIRES_MSG_T(concepts::boundable<aux::type<T...>>
                           , core::injector<::BOOST_DI_CFG, core::pool<>, T...>) {
public:
    #if defined(BOOST_DI_MSVC)
        #define BOOST_DI_CORE_INJECTOR(TConfig, TArgs) \
            core::injector<TConfig, core::pool<>, TArgs>
    #else
        #define BOOST_DI_CORE_INJECTOR(TConfig, TArgs) \
            core::injector<TConfig, decltype(((TConfig*)0)->policies()), TArgs>
    #endif

    template<
        class TConfig
      , class... TArgs
        #if defined(BOOST_DI_GCC)
          , BOOST_DI_REQUIRES_MSG(concepts::boundable<aux::type<T...>>)
        #endif
    > injector(const BOOST_DI_CORE_INJECTOR(TConfig, TArgs...)& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, core::pool<>, T...>(injector) {
            #if !defined(BOOST_DI_MSVC)
                using namespace detail;
                int _[]{0, (
                    create<T>(
                        std::integral_constant<bool,
                            core::is_creatable_impl<
                                typename std::is_same<concepts::configurable<TConfig>, std::true_type>::type
                              , core::injector<TConfig, decltype(((TConfig*)0)->policies()), TArgs...>
                              , T
                            >::value
                        >{}
                    )
                , 0)...}; (void)_;
            #endif
    }

    #undef BOOST_DI_CORE_INJECTOR
};

}}} // boost::di::v1

#endif

