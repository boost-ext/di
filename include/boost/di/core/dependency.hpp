//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_DEPENDENCY_HPP
#define BOOST_DI_CORE_DEPENDENCY_HPP

#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/array.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/concepts/scopable.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

BOOST_DI_HAS_METHOD(has_configure, configure);
BOOST_DI_HAS_TYPE(has_deps, deps);

template<class T, class U = std::remove_reference_t<T>>
struct is_injector
    : std::integral_constant<bool, has_deps<U>::value || has_configure<U>::value>
{ };

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
  , class TBase = TExpected
> class dependency : dependency_base, TScope::template scope<TExpected, TGiven>
    , public dependency_impl<
          dependency_concept<TBase, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority, TBase>
      > {
    template<class, class, class, class, class, class> friend class dependency;
    using scope_t = typename TScope::template scope<TExpected, TGiven>;

    template<class T>
    using externable = std::integral_constant<bool,
        !is_injector<T>::value &&
        std::is_same<TScope, scopes::deduce>::value &&
        std::is_same<TExpected, TGiven>::value
    >;

    template<class T>
    using specific = std::integral_constant<bool,
       is_injector<T>::value ||
       aux::is_array<TExpected, T>::value
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
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;
    using priority = TPriority;
    using base = TBase;

    dependency() noexcept { }

    template<class T>
    explicit dependency(T&& object) noexcept
        : scope_t(static_cast<T&&>(object))
    { }

    template<
        class TScope_
      , class TExpected_
      , class TGiven_
      , class TName_
      , class TPriority_
      , class TBase_
    > dependency(const dependency<TScope_, TExpected_, TGiven_, TName_, TPriority_, TBase_>& other) noexcept
        : scope_t(other)
    { }

    template<class T, BOOST_DI_REQUIRES(std::is_same<TName, no_name>::value && !std::is_same<T, no_name>::value) = 0>
    auto named() const noexcept {
        return dependency<
            TScope
          , TExpected
          , TGiven
          , T
          , TPriority
          , TBase
        >{*this};
    }

    template<class T, BOOST_DI_REQUIRES(std::is_same<TName, no_name>::value && !std::is_same<T, no_name>::value) = 0>
    auto named(const T&) const noexcept {
        return dependency<
            TScope
          , TExpected
          , TGiven
          , T
          , TPriority
          , TBase
        >{*this};
    }

    template<class T, BOOST_DI_REQUIRES_MSG(concepts::scopable<T>) = 0>
    auto in(const T&) const noexcept {
        return dependency<
            T
          , TExpected
          , TGiven
          , TName
          , TPriority
          , TBase
        >{};
    }

    template<class T, BOOST_DI_REQUIRES(!specific<T>::value) = 0, BOOST_DI_REQUIRES_MSG(typename concepts::boundable__<TExpected, T>::type) = 0>
    auto to() const noexcept {
        return dependency<
            TScope
          , TExpected
          , T
          , TName
          , TPriority
          , TBase
        >{};
    }

    template<class... Ts, BOOST_DI_REQUIRES(aux::is_array<TExpected, Ts...>::value) = 0>
    auto to() const noexcept {
        return dependency<
            TScope
          , TExpected
          , array<_, Ts...>
          , TName
          , TPriority
          , TBase
        >{};
    }

    template<class T, BOOST_DI_REQUIRES(externable<T>::value && !aux::is_narrowed<TExpected, T>::value || std::is_same<TExpected, _>::value) = 0>
    auto to(T&& object) const noexcept {
        using dependency = dependency<
            scopes::external
          , std::conditional_t<std::is_same<TExpected, _>::value, typename ref_traits<T>::type, TExpected>
          , typename ref_traits<T>::type
          , TName
          , TPriority
          , TBase
        >;
        return dependency{static_cast<T&&>(object)};
    }

    template<class T, BOOST_DI_REQUIRES(has_configure<T>::value) = 0>
    auto to(const T& object = {}) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>
          , TExpected
          , decltype(std::declval<T>().configure())
          , TName
          , TPriority
          , TBase
        >;
        return dependency{object.configure()};
    }

    template<class T, BOOST_DI_REQUIRES(has_deps<T>::value) = 0>
    auto to(const T& object = {}) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>
          , TExpected
          , T
          , TName
          , TPriority
          , TBase
        >;
        return dependency{object};
    }

    template<class T>
    auto to(std::initializer_list<T>&& object) const noexcept {
        using dependency = dependency<
            scopes::external
          , TExpected
          , std::initializer_list<T>
          , TName
          , TPriority
          , TBase
        >;
        return dependency{object};
    }

    auto operator[](const override&) const noexcept {
        return dependency<
            TScope
          , TExpected
          , TGiven
          , TName
          , override
        >{*this};
    }

    #if defined(__cpp_variable_templates) // __pph__
        const dependency& operator()() const noexcept {
            return *this;
        }
    #endif // __pph__

protected:
    using scope_t::is_referable;
    using scope_t::create;
    using scope_t::try_create;
    template<class> static void try_create(...);
};

template<class T>
struct is_dependency : std::is_base_of<dependency_base, T> { };

}}}} // boost::di::v1::core

#endif

