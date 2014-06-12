//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DEPENDENCY_HPP
#define BOOST_DI_CONCEPTS_DEPENDENCY_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/wrappers/reference.hpp"
#include "boost/di/wrappers/value.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/type_traits/parameter_types.hpp"
#include "boost/di/type_traits/has_call_operator.hpp"
#include "boost/di/concepts/detail/requires.hpp"
#include "boost/di/concepts/type_traits/is_required_priority.hpp"
#include "boost/di/concepts/type_traits/is_required_type.hpp"
#include "boost/di/type_traits/create_traits.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost {
namespace di {
namespace concepts {

namespace detail {

template<typename T>
struct scope_traits
{
    typedef T type;
};

template<>
struct scope_traits<mpl::_1>
{
    typedef scopes::deduce type;
};

template<
    typename TExpected
  , typename TGiven
  , typename TScope
>
struct get_scope
    : detail::scope_traits<TScope>::type::template
         scope<TExpected, TGiven>
{
    get_scope() { }
};

} // namespace detail

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TBind =
        detail::requires_<
            concepts::type_traits::is_required_priority
          , concepts::type_traits::is_required_type<TExpected>
        >
>
class dependency : public detail::get_scope<TExpected, TGiven, TScope>::type
{
    typedef typename detail::get_scope<TExpected, TGiven, TScope>::type scope_type;
    typedef scopes::external<wrappers::reference> ref_type;
    typedef scopes::external<wrappers::shared> shared_type;
    typedef scopes::external<wrappers::value> value_type;

    template<typename>
    struct get_wrapper_impl
    {
        typedef value_type type;
    };

    template<typename T>
    struct get_wrapper_impl<reference_wrapper<T> >
    {
        typedef ref_type type;
    };

    template<typename T>
    struct get_wrapper_impl<aux::shared_ptr<T> >
    {
        typedef shared_type type;
    };

    template<typename T, typename = void>
    struct get_wrapper
    {
        typedef T type;
    };

    template<typename T>
    struct get_wrapper<T, typename enable_if<di::type_traits::has_call_operator<T> >::type>
        : get_wrapper_impl<
              typename di::type_traits::parameter_types<
                  BOOST_DI_FEATURE_DECLTYPE(&T::operator())
              >::result_type
          >
    { };

public:
    typedef dependency type;
    typedef typename detail::scope_traits<TScope>::type scope;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TBind bind;

    template<typename T>
    struct rebind
    {
        typedef dependency<
            typename mpl::if_<
                is_same<scope, scopes::deduce>
              , T
              , TScope
            >::type
          , TExpected
          , TGiven
          , TBind
        > other;
    };

    dependency() { }

    template<typename T>
    explicit dependency(const T& object)
        : scope_type(object)
    { }

    template<typename T>
    static dependency<value_type, expected, T, TBind>
    to(const T& object, typename disable_if<is_reference_wrapper<T> >::type* = 0
                      , typename disable_if<di::type_traits::has_call_operator<T> >::type* = 0) {
        return dependency<value_type, expected, T, TBind>(object);
    }

    template<typename T>
    static dependency<ref_type, typename unwrap_reference<T>::type, T, TBind>
    to(const T& object, typename enable_if<is_reference_wrapper<T> >::type* = 0
                      , typename disable_if<di::type_traits::has_call_operator<T> >::type* = 0) {
        return dependency<ref_type, typename unwrap_reference<T>::type, T, TBind>(object);
    }

    template<typename T>
    static dependency<typename get_wrapper<T>::type, expected, T, TBind>
    to(const T& object, typename disable_if<is_reference_wrapper<T> >::type* = 0
                      , typename enable_if<di::type_traits::has_call_operator<T> >::type* = 0) {
        return dependency<typename get_wrapper<T>::type, expected, T, TBind>(object);
    }

    template<typename T>
    static dependency<shared_type, expected, T>
    to(const aux::shared_ptr<T>& object) {
        return dependency<shared_type, expected, T>(object);
    }
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

