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
#include "boost/di/config.hpp"

#include <boost/di/scopes/external.hpp>
#include <boost/di/type_traits/is_same_base_of.hpp>

namespace boost {
namespace di {
namespace concepts {

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TContext = mpl::vector0<>
  , typename TBind = is_same<mpl::_1, TExpected>
>
class dependency : public TScope::template scope<TExpected, TGiven>
{
    typedef typename TScope::template scope<TExpected, TGiven> scope_type;

public:
    typedef dependency type;
    typedef TScope scope;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TContext context;
    typedef TBind bind;
    typedef typename scope_type::result_type result_type;

public:
    dependency() { }

    template<typename T>
    explicit dependency(const T& obj)
        : scope_type(obj)
    { }

    template<typename T>
    struct rebind
    {
        typedef dependency<
            typename mpl::if_<is_same<TScope, scopes::deduce>, T, TScope>::type
          , TExpected
          , TGiven
          , TContext
          , TBind
        > other;
    };

    template<typename TValue>
    static dependency<
        scopes::external
      , TExpected
      , TValue
      , TContext
      , type_traits::is_same_base_of<TExpected>
    >
    to(const TValue& value) {
        return dependency<
            scopes::external
          , TExpected
          , TValue
          , TContext
          , type_traits::is_same_base_of<TExpected>
        >(value);
    }

    template<typename TValue>
    static dependency<
        scopes::external
      , TExpected
      , TValue
      , TContext
      , type_traits::is_same_base_of<TExpected>
    >
    to(TValue& value) {
        return dependency<
            scopes::external
          , TExpected
          , TValue
          , TContext
          , type_traits::is_same_base_of<TExpected>
        >(value);
    }

    template<typename TValue>
    static dependency<
        scopes::external
      , TExpected
      , TValue
      , TContext
      , type_traits::is_same_base_of<TExpected>
    >
    to(boost::shared_ptr<TValue> value) {
        return dependency<
            scopes::external
          , TExpected
          , TValue
          , TContext
          , type_traits::is_same_base_of<TExpected>
        >(value);
    }
};

template<
    typename TExpected
  , typename TGiven
  , typename TContext
  , typename TBind
>
class dependency<
    mpl::_1
  , TExpected
  , TGiven
  , TContext
  , TBind
>
{
public:
    template<typename TScope>
    struct rebind
    {
        typedef dependency<
            TScope
          , TExpected
          , TGiven
          , TContext
          , TBind
        > other;
    };

    template<typename TValue>
    static dependency<
        scopes::external
      , TExpected
      , TValue
      , TContext
      , type_traits::is_same_base_of<TExpected>
    >
    to(const TValue& value) {
        return dependency<
            scopes::external
          , TExpected
          , TValue
          , TContext
          , type_traits::is_same_base_of<TExpected>
        >(value);
    }

    template<typename TValue>
    static dependency<
        scopes::external
      , TExpected
      , TValue
      , TContext
      , type_traits::is_same_base_of<TExpected>
    >
    to(TValue& value) {
        return dependency<
            scopes::external
          , TExpected
          , TValue
          , TContext
          , type_traits::is_same_base_of<TExpected>
        >(value);
    }

    template<typename TValue>
    static dependency<
        scopes::external
      , TExpected
      , TValue
      , TContext
      , type_traits::is_same_base_of<TExpected>
    >
    to(boost::shared_ptr<TValue> value) {
        return dependency<
            scopes::external
          , TExpected
          , TValue
          , TContext
          , type_traits::is_same_base_of<TExpected>
        >(value);
    }
};

template<
    typename TScope
  , typename TBind
  , typename TContext
>
class dependency<
    TScope
  , mpl::_1
  , mpl::_2
  , TContext
  , TBind
>
{
public:
    template<
        typename TExpected
      , typename TGiven
    >
    struct rebind
    {
        typedef dependency<
            TScope
          , TExpected
          , TGiven
          , TContext
          , TBind
        > other;
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

