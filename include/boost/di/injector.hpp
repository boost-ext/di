//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/core/injector.hpp"
#include "boost/di/config.hpp"

namespace boost { namespace di {

//template<class T, class D>
//struct get {
    //using type = decltype(concepts::creatable(std::declval<typename T::given>(), std::declval<D>(), std::declval<TConfig>().policies()));
//};

//template<class, class>
//struct creatable_for_all;

//template<class T, class... TArgs>
//struct creatable_for_all<T, aux::type_list<TArgs...>> {
    //using type = std::is_same<aux::bool_list<aux::always<TArgs>{}...>, aux::bool_list<typename get<TArgs, T>::type{}...>>;
//};

template<class TConfig, class... TDeps>
class injector_ : public core::injector<TConfig, TDeps...> {
    using self = core::injector<TConfig, TDeps...>;

public:
    using deps = typename self::deps;

    template<class... TArgs>
    explicit injector_(const TArgs&... args) noexcept
        : self{core::init{}, core::pass_arg(args)...}
    { }

    template<class TConfig_, class... TDeps_>
    injector_(const core::injector<TConfig_, TDeps_...>& injector
            //, BOOST_DI_REQUIRES_OVERLOAD(typename creatable_for_all<TDeps_, TDeps>::type{})
    ) noexcept // non explicit
        : self{injector}
    { }

    template<
        class T
      , BOOST_DI_REQUIRES(concepts::creatable(std::declval<T>(), std::declval<deps>(), std::declval<TConfig>().policies()))
    > T create() const {
        return self::template create_impl<T>();
    }

    template<class TAction>
    void call(const TAction& action) noexcept {
        self::call_impl(action, deps{});
    }
};

template<class... TDeps>
using injector = injector_<config, TDeps...>;

}} // boost::di

#endif

