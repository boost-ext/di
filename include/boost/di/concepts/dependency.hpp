//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DEPENDENCY_HPP
#define BOOST_DI_CONCEPTS_DEPENDENCY_HPP

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/convertibles/ref.hpp"
#include "boost/di/convertibles/shared.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"

#include <memory>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/has_xxx.hpp>

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

BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

template<typename T, typename = void>
struct named_traits
{
    typedef T type;
};

template<typename T>
struct named_traits<T, typename enable_if<has_named_type<T> >::type>
{
    typedef typename type_traits::remove_accessors<
        typename T::named_type
    >::type type;
};

} // namespace detail

template<
    typename TExpected
  , typename TGiven
  , typename TScope
>
struct scope_type
{
    typedef typename detail::scope_traits<TScope>::type::template
        scope<typename detail::named_traits<TExpected>::type
            , typename detail::named_traits<TGiven>::type
        >
    type;
};

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TContext = mpl::vector0<>
  , typename TBind = typename mpl::lambda<
        is_same<mpl::_1, TExpected>
    >::type
>
class dependency : public scope_type<TExpected, TGiven, TScope>::type
{
    typedef typename scope_type<TExpected, TGiven, TScope>::type scope_type;
    typedef scopes::external<convertibles::ref> ref_type;
    typedef scopes::external<convertibles::shared> shared_type;
    typedef scopes::external<convertibles::value> value_type;

    template<typename T, typename U, typename S>
    struct external
    {
        typedef dependency<S, T, U, TContext, TBind> type;
    };

    template<typename T>
    struct is_number
        : mpl::or_<is_arithmetic<T>, is_enum<T> >
    { };

public:
    typedef dependency type;
    typedef typename detail::scope_traits<TScope>::type scope;
    typedef typename detail::named_traits<TExpected>::type expected;
    typedef typename detail::named_traits<TGiven>::type given;
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
    to(shared_ptr<T> obj) {
        return typename external<expected, T, shared_type>::type(obj);
    }
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

