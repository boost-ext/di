//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DEPENDENCY_HPP
#define BOOST_DI_CONCEPTS_DEPENDENCY_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/convertibles/reference.hpp"
#include "boost/di/convertibles/shared.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/type_traits/parameter_types.hpp"
#include "boost/di/concepts/detail/requires.hpp"
#include "boost/di/concepts/type_traits/type.hpp"
#include "boost/di/concepts/type_traits/priority.hpp"

#include <string>
#include <boost/ref.hpp>
#include <boost/non_type.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

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

} // namespace detail

template<
    typename TExpected
  , typename TGiven
  , typename TScope
>
struct get_scope
{
    typedef typename detail::scope_traits<TScope>::type::
        template scope<TExpected, TGiven> type;
};

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TBind =
        detail::requires<
            concepts::type_traits::priority
          , concepts::type_traits::type<TExpected>
        >
>
class dependency : public get_scope<TExpected, TGiven, TScope>::type
{
    typedef typename get_scope<TExpected, TGiven, TScope>::type scope_type;
    typedef scopes::external<convertibles::reference> ref_type;
    typedef scopes::external<convertibles::shared> shared_type;
    typedef scopes::external<convertibles::value> value_type;

    template<typename T, typename U, typename S>
    struct external
    {
        typedef dependency<S, T, U, TBind> type;
    };

    template<typename T>
    class has_call_operator
    {
        struct base_impl { void operator()(...) { } };
        struct base
            : base_impl
            , mpl::if_<is_class<T>, T, mpl::void_>::type
        { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<void (base_impl::*)(...), &U::operator()>* = 0
        );

        static mpl::aux::yes_tag test(...);

    public:
        typedef has_call_operator type;

        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename, typename = void>
    struct is_text
        : mpl::false_
    { };

    template<typename T, int N>
    struct is_text<T[N], void>
        : mpl::true_
    { };

    template<typename T>
    struct is_text<std::string, T>
        : mpl::true_
    { };

    template<typename T>
    struct is_text<const char*, T>
        : mpl::true_
    { };

    template<typename T>
    struct is_text<char*, T>
        : mpl::true_
    { };

    template<typename T>
    struct is_value
        : mpl::or_<is_arithmetic<T>, is_enum<T>, is_text<T> >
    { };

    template<typename T>
    struct get_convertible_impl;

    template<typename T>
    struct get_convertible_impl<T&>
    {
        typedef ref_type type;
    };

    template<typename T>
    struct get_convertible_impl<aux::shared_ptr<T> >
    {
        typedef shared_type type;
    };

    template<typename T, typename = void>
    struct get_convertible
    {
        typedef T type;
    };

    template<typename T>
    struct get_convertible<T, typename enable_if<has_call_operator<T> >::type>
        : get_convertible_impl<typename di::type_traits::parameter_types<BOOST_TYPEOF_TPL(&T::operator())>::result_type>
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
    explicit dependency(const T& obj)
        : scope_type(obj)
    { }

    template<typename T>
    static typename external<expected, T, value_type>::type
    to(const T& obj, typename enable_if<is_value<T> >::type* = 0
                   , typename disable_if<has_call_operator<T> >::type* = 0) {
        return typename external<expected, T, value_type>::type(obj);
    }

    template<typename T>
    static typename external<const expected, T, ref_type>::type
    to(const T& obj, typename disable_if<is_value<T> >::type* = 0
                   , typename disable_if<has_call_operator<T> >::type* = 0) {
        return typename external<const expected, T, ref_type>::type(boost::cref(obj));
    }

    template<typename T>
    static typename external<expected, T, typename get_convertible<T>::type>::type
    to(const T& obj, typename enable_if<has_call_operator<T> >::type* = 0
                   , typename disable_if<is_value<T> >::type* = 0) {
        return typename external<expected, T, typename get_convertible<T>::type>::type(obj);
    }

    template<typename T>
    static typename external<expected, T, ref_type>::type
    to(T& obj) {
        return typename external<expected, T, ref_type>::type(boost::ref(obj));
    }

    template<typename T>
    static typename external<expected, T, shared_type>::type
    to(aux::shared_ptr<T> obj) {
        return typename external<expected, T, shared_type>::type(obj);
    }
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

