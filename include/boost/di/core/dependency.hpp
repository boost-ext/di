//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_DEPENDENCY_HPP
#define BOOST_DI_CORE_DEPENDENCY_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/concepts/scopable.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_TYPE(deps);

template<class T, class U = std::remove_reference_t<T>>
struct is_injector :
    std::integral_constant<bool, has_deps<U>::value || has_configure<U>::value>{};

template<class, class>
struct dependency_concept { };

template<class T, class TDependency>
struct dependency_impl
    : aux::pair<T, TDependency>
{ };

template<class... Ts, class TName, class TDependency>
struct dependency_impl<
    dependency_concept<aux::type_list<Ts...>, TName>
  , TDependency
> : aux::pair<dependency_concept<Ts, TName>, TDependency>...
{ };

struct override { };

struct dependency_base { };

template<
    class TScope
  , class TExpected
  , class TGiven = TExpected
  , class TName = no_name
  , class TPriority = aux::none_type
> class dependency
    : dependency_base
    , public TScope::template scope<TExpected, TGiven>
    , public dependency_impl<
          dependency_concept<TExpected, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority>
      > {
    template<class T>
    using is_not_narrowed = std::integral_constant<bool,
        (std::is_arithmetic<T>::value && std::is_same<TExpected, T>::value) || !std::is_arithmetic<T>::value
    >;

    template<class T>
    using externable = std::integral_constant<bool,
        !is_injector<T>::value &&
        std::is_same<TScope, scopes::deduce>::value &&
        std::is_same<TExpected, TGiven>::value &&
        is_not_narrowed<T>::value
    >;

    template<class T>
    struct ref_traits {
        using type = T;
    };

    template<int N>
    struct ref_traits<const char(&)[N]> {
        using type = TExpected;
    };

    template<class T>
    struct ref_traits<std::shared_ptr<T>&> {
        using type = std::shared_ptr<T>;
    };

public:
    using creator = typename TScope::template scope<TExpected, TGiven>;
    using scope = TScope;
    using expected = TExpected;
    using given = std::remove_reference_t<TGiven>;
    using name = TName;
    using priority = TPriority;

    dependency() noexcept { }

    template<class T>
    explicit dependency(T&& object) noexcept
        : creator(static_cast<T&&>(object))
    { }

    template<
        class TScope_
      , class TExpected_
      , class TGiven_
      , class TName_
      , class TPriority_
    > dependency(const dependency<TScope_, TExpected_, TGiven_, TName_, TPriority_>& other) noexcept
        : creator(other)
    { }

    template<class T> // no requirements
    auto named(const T&) const noexcept {
        return dependency<TScope, TExpected, TGiven, T>{*this};
    }

    template<class T, BOOST_DI_REQUIRES(concepts::scopable<T>::value)>
    auto in(const T&) const noexcept {
        return dependency<T, TExpected, TGiven, TName>{};
    }

    template<class T, BOOST_DI_REQUIRES(externable<T>::value)>
    auto to(T&& object) const noexcept {
        using dependency = dependency<
            scopes::external, TExpected, typename ref_traits<T>::type, TName
        >;
        return dependency{static_cast<T&&>(object)};
    }

    template<class T, BOOST_DI_REQUIRES(has_configure<T>::value)>
    auto to(const T& object) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, decltype(std::declval<T>().configure()), TName
        >;
        return dependency{object.configure()};
    }

    template<class T, BOOST_DI_REQUIRES(has_deps<T>::value)>
    auto to(const T& object) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, T, TName
        >;
        return dependency{object};
    }

    auto operator[](const override&) const noexcept {
        return dependency<TScope, TExpected, TGiven, TName, override>{*this};
    }

    #if defined(__cpp_variable_templates)
        const dependency& operator()() const noexcept {
            return *this;
        }
    #endif
};

template<class T>
struct is_dependency : std::is_base_of<dependency_base, T> { };

}}}} // boost::di::v1::core

#endif

