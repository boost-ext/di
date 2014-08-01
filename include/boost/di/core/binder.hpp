//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDER_HPP
#define BOOST_DI_CORE_BINDER_HPP

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"
#include "boost/di/wrappers/universal.hpp"
#include "boost/di/bindings/dependency.hpp"
#include "boost/di/scopes/deduce.hpp"

#include <boost/mpl/if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/greater.hpp>

namespace boost {
namespace di {
namespace core {

template<typename TDependecies>
class binder
{
    template<
        typename T
      , typename TCallStack
      , typename TDependency
    >
    struct data
    {
        typedef T type;
        typedef TCallStack call_stack;
        typedef TDependency dependency;
    };

    template<
        typename TDependency
      , typename T
      , typename TCallStack
    >
    struct apply
        : TDependency::bind::template apply<data<T, TCallStack, TDependency> >::type
    { };

public:
    template<
        typename T
      , typename TCallStack
      , typename TDefault =
            bindings::dependency<
                scopes::deduce
              , typename type_traits::make_plain<T>::type
            >
    >
    struct resolve
        : aux::mpl::deref<
              aux::mpl::second<
                  typename aux::mpl::fold<
                      TDependecies
                    , aux::mpl::pair<aux::mpl::int_<0>, TDefault>
                    , aux::mpl::if_<
                          aux::mpl::greater<
                              apply<
                                  aux::mpl::_2
                                , T
                                , TCallStack
                              >
                            , aux::mpl::first<aux::mpl::_1>
                          >
                        , aux::mpl::pair<apply<aux::mpl::_2, T, TCallStack>, aux::mpl::_2>
                        , aux::mpl::_1
                      >
                  >::type
              >
          >::type::template
              rebind<typename scopes::deduce::rebind<T>::other>::other
    { };
};

} // namespace core
} // namespace di
} // namespace boost

#endif

