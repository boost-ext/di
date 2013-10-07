//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_HPP
#define BOOST_DI_CONCEPTS_HPP

#include <memory>
#include <string>
#include <vector>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/mpl/int.hpp>
#include "boost/di/detail/meta.hpp"
#include "boost/di/concepts/bind.hpp"
#include "boost/di/concepts/dependency.hpp"
#include "boost/di/concepts/scope.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/per_request.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/named.hpp"
#include "boost/di/ctor.hpp"

namespace boost {
namespace di {

template<int N>
struct int_ : mpl::int_<N>
{
    operator int() const { return N; }
};

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct call_stack
    : mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename TScope>
struct scope
    : concepts::scope<TScope, concepts::dependency, named>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct deduce
    : scope<scopes::deduce>::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct per_request
    : scope<scopes::per_request<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct singleton
    : scope<scopes::singleton<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct session
    : scope<scopes::session<> >::bind<BOOST_DI_TYPES_PASS_MPL(T)>
{ };

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : concepts::bind<TExpected, TGiven, concepts::dependency, named>
{ };

template<int N>
struct bind_int
    : bind<int, int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

template<typename TExpected>
struct bind_vector
{
    template<template<typename> class, typename, typename = void>
    struct make_vector;

    #define CCC(_, n, na) BOOST_PP_COMMA_IF(n) T<typename mpl::at_c<TSeq, n>::type> args##n
    #define DDD(_, n, na) this->push_back(args##n);

    #define BOOST_PP_LOCAL_MACRO(n)                                         \
        template<template<typename> class T, typename TSeq>
        struct make_vector<T, TSeq, typename enable_if_c<mpl::size<TSeq>::value == n>::type>
            : std::vector<T<TExpected> >
        {
            BOOST_DI_CTOR(make_vector, BOOST_PP_REPEAT(n, CCC, ~)) {
                BOOST_PP_REPEAT(n, DDD, ~)
            }
        }

    #define BOOST_PP_LOCAL_LIMITS (1, 5)
    #include BOOST_PP_LOCAL_ITERATE()

    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct to
        : scope<scopes::deduce>::bind<
             bind<std::vector<TExpected>, make_vector1<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> > >
           , bind<std::vector<shared_ptr<TExpected> >, make_vector<boost::shared_ptr, mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> > >
          >
    { };
};

} // namespace di
} // namespace boost

#endif

