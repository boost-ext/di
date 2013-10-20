//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DEPENDENCY_HPP
#define BOOST_DI_CONCEPTS_DEPENDENCY_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/convertibles/ref.hpp"
#include "boost/di/convertibles/shared.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"

#include <memory>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/lambda.hpp>

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
  , typename TName = void
  , typename TContext = mpl::vector0<>
  , typename TBind = typename mpl::lambda<
        mpl::and_<
            is_same<TExpected, mpl::_1>
          , is_same<TName, mpl::_2>
        >
    >::type
>
class dependency : public get_scope<TExpected, TGiven, TScope>::type
{
    typedef typename get_scope<TExpected, TGiven, TScope>::type scope_type;
    typedef scopes::external<convertibles::ref> ref_type;
    typedef scopes::external<convertibles::shared> shared_type;
    typedef scopes::external<convertibles::value> value_type;

    template<typename T, typename U, typename S>
    struct external
    {
        typedef dependency<S, T, U, TName, TContext, TBind> type;
    };

    template<typename T>
    struct is_number
        : mpl::or_<is_arithmetic<T>, is_enum<T> >
    { };

public:
    typedef dependency type;
    typedef typename detail::scope_traits<TScope>::type scope;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TName name;
    typedef TContext context;
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
          , TName
          , TContext
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
    to(const T& obj, typename enable_if<is_number<T> >::type* = 0) {
        return typename external<expected, T, value_type>::type(obj);
    }

    template<typename T>
    static typename external<const expected, T, ref_type>::type
    to(const T& obj, typename disable_if<is_number<T> >::type* = 0) {
        return typename external<const expected, T, ref_type>::type(boost::cref(obj));
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

