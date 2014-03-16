//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_HPP
#define BOOST_DI_CONCEPTS_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/concepts/bind.hpp"
#include "boost/di/concepts/dependency.hpp"
#include "boost/di/concepts/scope.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"

#include <string>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/max.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>

namespace boost {
namespace di {

template<typename TScope>
struct scope
    : concepts::scope<TScope, concepts::dependency>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct deduce
    : scope<scopes::deduce>::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct unique
    : scope<scopes::unique<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct shared
    : scope<scopes::shared<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct session
    : scope<scopes::session<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : concepts::bind<TExpected, TGiven, concepts::dependency>
{ };

template<int N>
struct bind_int
    : bind<int, mpl::int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class call_stack
{
public:
    typedef mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> context_type;

    template<typename TContext, typename TCallStack>
    struct apply_impl
        : mpl::if_<
              mpl::empty<TCallStack>
            , mpl::int_<0>
            , mpl::if_<
                  mpl::equal<
                      mpl::iterator_range<
                          typename mpl::advance<
                              typename mpl::begin<TCallStack>::type
                            , typename mpl::max<
                                  mpl::int_<0>
                                , mpl::minus<
                                      mpl::size<TCallStack>
                                    , mpl::size<TContext>
                                  >
                              >::type
                          >::type
                        , typename mpl::end<TCallStack>::type
                      >
                    , TContext
                  >
                , mpl::size<TContext>
                , mpl::int_<0>
              >
          >
    { };

public:
    template<typename, typename TCallStack, typename>
    struct apply
        : apply_impl<context_type, TCallStack>::type
    { };
};

} // namespace di
} // namespace boost

#endif

