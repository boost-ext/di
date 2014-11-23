// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_DEPENDENCY_HPP
#define BOOST_DI_CORE_DEPENDENCY_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/wrappers/reference.hpp"
#include "boost/di/wrappers/value.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/named.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
namespace core {

template<typename TExpected, typename TName>
struct dependency_concept { };

template<typename T, typename TDependency>
struct dependency_impl : pair<T, TDependency>
{ };

template<typename... Ts, typename TName, typename TDependency>
struct dependency_impl<dependency_concept<type_list<Ts...>, TName>, TDependency>
	: pair<dependency_concept<Ts, TName>, TDependency>...
{ };

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TName = no_name
>
class dependency
    : public TScope::template scope<TGiven>
    , public dependency_impl<
          dependency_concept<TExpected, TName>
        , dependency<TScope, TExpected, TGiven, TName>
      > {
public:
    using type = dependency;
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;

    template<typename T>
    struct rebind {
        using other = dependency<
            aux::conditional_t<
                std::is_same<scope, scopes::deduce>{}
              , T
              , TScope
            >
          , TExpected
          , TGiven
          , TName
        >;
    };

    dependency() { }

    template<typename T>
    explicit dependency(T&& object)
        : TScope::template scope<TGiven>(std::forward<T>(object))
    { }

    template<typename T, typename TInjector>
    dependency(T&& object, TInjector& injector)
        : TScope::template scope<TGiven>(std::forward<T>(object), injector)
    { }

    template<typename T>
    auto named(const T&) {
        return dependency<TScope, TExpected, TGiven, T>{};
    }

    template<typename T>
    auto in(const T&) {
        return dependency<T, TExpected, TGiven, TName>{};
    }

    template<typename T>
    auto to(T&& object) {
        return dependency<scopes::external<T>, TExpected, TExpected, TName>{std::forward<T>(object)};
    }
};

} // namespace core
} // namespace di
} // namespace boost

#endif

