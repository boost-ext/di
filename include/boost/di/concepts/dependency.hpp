//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DEPENDENCY_HPP
#define BOOST_DI_CONCEPTS_DEPENDENCY_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/di/scopes/deduce.hpp>
#include <boost/di/scopes/external.hpp>
#include "boost/di/config.hpp"

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
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TContext = mpl::vector0<>
  , typename TBind = is_same<mpl::_1, TExpected>
>
class dependency
    : public detail::scope_traits<TScope>::type::template scope<TExpected, TGiven>
{
    template<typename T>
    struct external
    {
        typedef dependency<
            scopes::external,
            TExpected
          , T
          , TContext
          , TBind
        > type;
    };

public:
    typedef dependency type;
    typedef typename detail::scope_traits<TScope>::type scope;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TContext context;
    typedef TBind bind;

    dependency() { }

    template<typename T>
    explicit dependency(const T& obj)
        : detail::scope_traits<TScope>::type::template scope<TExpected, TGiven>(obj)
    { }

    template<typename T>
    struct rebind
    {
        typedef dependency<
            typename mpl::if_<
                is_same<scope, scopes::deduce>
              , T
              , scope
            >::type
          , TExpected
          , TGiven
          , TContext
          , TBind
        > other;
    };

    template<typename T>
    static typename external<T>::type to(const T& obj) {
        return typename external<T>::type(obj);
    }

    template<typename T>
    static typename external<T>::type to(T& obj) {
        return typename external<T>::type(obj);
     }

    template<typename T>
    static typename external<T>::type to(shared_ptr<T> obj) {
        return typename external<T>::type(obj);
     }

};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

