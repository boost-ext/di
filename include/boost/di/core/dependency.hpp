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

BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

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
class dependency : public TScope::template scope<TGiven>,
                   public dependency_impl<dependency_concept<TExpected, TName>, dependency<TScope, TExpected, TGiven, TName>> {
    using scope_type = typename TScope::template scope<TGiven>;
    using ref_type = scopes::external<wrappers::reference>;
    using shared_type = scopes::external<wrappers::shared>;
    using value_type = scopes::external<wrappers::value>;

    template<typename, typename = void>
    struct get_wrapper_impl
    {
        typedef value_type type;
    };

    template<typename T>
    struct get_wrapper_impl<aux::shared_ptr<T> >
    {
        typedef shared_type type;
    };

    template<typename T>
    struct get_wrapper_impl<T, typename enable_if<is_reference_wrapper<T> >::type>
    {
        typedef ref_type type;
    };

    template<
        typename T
      , typename = void
      , typename = void
      , typename = void
      , typename = void
    >
    struct get_wrapper
    {
        typedef T type;
    };

    template<typename T>
    struct get_wrapper<T, typename enable_if<has_result_type<T> >::type>
        : get_wrapper_impl<typename T::result_type>
    { };

    template<typename T>
    struct get_wrapper<T, typename enable_if<aux::has_call_operator<T> >::type
                        , typename disable_if<has_result_type<T> >::type
                        , typename disable_if<aux::is_reference_wrapper<T> >::type
                        , typename disable_if<aux::has_lambda<T, fwd::injector&> >::type>
        : get_wrapper_impl<typename aux::function_traits<decltype(&T::operator())>::result_type>
    { };

    template<typename T>
    struct get_wrapper<T, typename enable_if<aux::has_lambda<T, fwd::injector&> >::type
                        , typename disable_if<has_result_type<T> >::type
                        , typename disable_if<aux::is_reference_wrapper<T> >::type>
        : get_wrapper_impl<
              typename aux::function_traits<
                  decltype(&T::template operator()<fwd::injector>)
              >::result_type
          >
    { };

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
    explicit dependency(const T& object)
        : scope_type(object)
    { }

    template<typename T, typename TInjector>
    dependency(const T& object, TInjector& injector)
        : scope_type(object, injector)
    { }

    template<typename T>
    dependency<value_type, expected, T, TName>
    to(const T& object, typename disable_if<aux::is_reference_wrapper<T> >::type* = 0
                      , typename disable_if<aux::has_call_operator<T> >::type* = 0) {
        return dependency<value_type, expected, T, TName>(object);
    }

    template<typename T>
    dependency<ref_type, typename unwrap_reference<T>::type, T, TName>
    to(const T& object, typename enable_if<aux::is_reference_wrapper<T> >::type* = 0) {
        return dependency<ref_type, typename unwrap_reference<T>::type, T, TName>(object);
    }

    template<typename T>
    dependency<typename get_wrapper<T>::type, expected, T, TName>
    to(const T& object, typename disable_if<aux::is_reference_wrapper<T> >::type* = 0
                      , typename enable_if<aux::has_call_operator<T> >::type* = 0) {
        return dependency<typename get_wrapper<T>::type, expected, T, TName>(object);
    }

    template<typename T>
    dependency<shared_type, expected, T, TName>
    to(const aux::shared_ptr<T>& object) {
        return dependency<shared_type, expected, T, TName>(object);
    }

    template<typename TNam>
    dependency<TScope, TExpected, TGiven, TNam>
    named(const TNam&) { return {}; }

    template<typename TSc>
    dependency<TSc, TExpected, TGiven, TName>
    in(const TSc&) { return {}; }

};

} // namespace core
} // namespace di
} // namespace boost

#endif

