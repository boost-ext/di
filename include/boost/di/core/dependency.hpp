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

namespace boost {
namespace di {
namespace core {

template<class TExpected, class TName>
struct dependency_concept { };

template<class T, class TDependency>
struct dependency_impl : pair<T, TDependency>
{ };

template<class... Ts, class TName, class TDependency>
struct dependency_impl<dependency_concept<type_list<Ts...>, TName>, TDependency>
    : pair<dependency_concept<Ts, TName>, TDependency>...
{ };

template<
    class TScope
  , class TExpected
  , class TGiven = TExpected
  , class TName = no_name
  , bool  TPriority = TScope::priority
>
class dependency
    : public TScope::template scope<TExpected, TGiven>
    , public dependency_impl<
          dependency_concept<TExpected, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority>
      > {
public:
    using type = dependency;
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;

    dependency() noexcept { }

    template<class T>
    explicit dependency(T&& object) noexcept
        : TScope::template scope<TExpected, TGiven>(std::forward<T>(object))
    { }

    template<class T, class TInjector>
    dependency(T&& object, const TInjector& injector) noexcept
        : TScope::template scope<TExpected, TGiven>(std::forward<T>(object), injector)
    { }

    template<class T>
    auto named(const T&) const noexcept {
        return dependency<TScope, TExpected, TGiven, T>{};
    }

    template<class T>
    auto in(const T&) const noexcept {
        return dependency<T, TExpected, TGiven, TName>{};
    }

    template<class T>
    auto to(T&& object) const noexcept {
        using dep = dependency<scopes::external, TExpected, aux::remove_accessors_t<T>, TName>;
        return dep{std::forward<T>(object)};
    }
};

template<class>
struct is_dependency : std::false_type { };

template<
    class TScope
  , class TExpected
  , class TGiven
  , class TName
  , bool  TPriority
>
struct is_dependency<
    dependency<TScope, TExpected, TGiven, TName, TPriority>
> : std::true_type { };

} // namespace core
} // namespace di
} // namespace boost

#endif

