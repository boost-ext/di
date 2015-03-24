//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_DEPENDENCY_HPP
#define BOOST_DI_CORE_DEPENDENCY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_TYPE(deps);

template<class T, class U = std::remove_reference_t<T>>
using is_injector =
    std::integral_constant<bool, has_deps<U>{} || has_configure<U>{}>;

template<class, class>
struct dependency_concept { };

template<class T, class TDependency>
struct dependency_impl : aux::pair<T, TDependency>
{ };

template<class... Ts, class TName, class TDependency>
struct dependency_impl<
    dependency_concept<aux::type_list<Ts...>, TName>
  , TDependency
> : aux::pair<dependency_concept<Ts, TName>, TDependency>...
{ };

template<
    class TScope
  , class TExpected
  , class TGiven = TExpected
  , class TName = no_name
  , class TPriority = std::integral_constant<bool, TScope::priority>
> class dependency;

struct dependency_ { };

template<class I, class Impl>
struct bind : dependency<scopes::deduce, I, Impl> {
    template<class T>
    struct named_ : dependency<scopes::deduce, I, Impl, T> {
        using dependency<scopes::deduce, I, Impl, T>::dependency;
    };
};

template<
    class TScope
  , class TExpected
  , class TGiven
  , class TName
  , class TPriority
> class dependency
    : public TScope::template scope<TExpected, TGiven>
    , public dependency_
    , public dependency_impl<
          dependency_concept<TExpected, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority>
      > {
    using scope_t = typename TScope::template scope<TExpected, TGiven>;

public:
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;

    dependency() noexcept { }

    template<class T>
    explicit dependency(T&& object) noexcept
        : scope_t{std::forward<T>(object)}
    { }

    template<class... Ts>
    dependency(const dependency<Ts...>& other) noexcept
        : scope_t(other)
    { }

    template<class T>
    auto named(const T&) const noexcept {
        //return typename bind<TExpected, TGiven>::template named_<T>{*this};
        return dependency<TScope, TExpected, TGiven, T>{*this};
    }

    template<class T>
    auto in(const T&) const noexcept {
        return dependency<T, TExpected, TGiven, TName>{};
    }

    template<class T
           , REQUIRES<!is_injector<T>{} &&
                      std::is_same<TExpected, TGiven>{} &&
                      std::is_same<TScope, scopes::deduce>{}> = 0
    > auto to(T&& object) const noexcept {
        using dependency = dependency<
            scopes::external, TExpected, std::remove_reference_t<T>, TName
        >;
        return dependency{std::forward<T>(object)};
    }

    template<class T, REQUIRES<has_configure<T>{}> = 0>
    auto to(const T& object) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, decltype(std::declval<T>().configure()), TName
        >;
        return dependency{object.configure()};
    }

    template<class T, REQUIRES<has_deps<T>{}> = 0>
    auto to(const T& object) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, T, TName
        >;
        return dependency{object};
    }
};

template<class T>
struct is_dependency : std::is_base_of<dependency_, T> { };

}}} // boost::di::core

#endif

