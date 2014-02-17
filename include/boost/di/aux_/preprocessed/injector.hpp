//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/preprocessed/detail/module.hpp"
#include "boost/di/aux_/preprocessed/detail/pool.hpp"
#include "boost/di/aux_/preprocessed/scopes/deduce.hpp"
#include "boost/di/aux_/preprocessed/concepts.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/has_xxx.hpp>

    namespace boost {
    namespace di {

    namespace detail {

    template<typename T>
    struct default_scope
        : scope<scopes::deduce>::bind<T>
    { };

    BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

    template<typename T>
    struct deps
    {
        typedef typename T::deps type;
    };

    template<typename TSeq>
    struct concepts
        : mpl::fold<
              TSeq
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      mpl::is_sequence<mpl::_2>
                    , mpl::_2
                    , mpl::if_<
                          has_deps<mpl::_2>
                        , deps<mpl::_2>
                        , default_scope<mpl::_2>
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >::type
    { };

    }

    template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
    class injector
        : public detail::module<
              typename detail::concepts<
                  mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
              >::type
          >
    {
        template<typename TSeq = mpl::vector0<> >
        struct joint_concepts
            : detail::concepts<
                  mpl::joint_view<
                      mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
                    , TSeq
                  >
              >::type
        { };

    public:
        injector() { }

        injector<> operator()() const {
            return injector<>();
        }

    template< typename Args0>
    explicit injector( Args0 args0)
        : detail::module<typename joint_concepts<>::type>( args0)
    { }

    template< typename Args0>
    injector<joint_concepts<mpl::vector< Args0> > >
    operator()( Args0 args0) const {
        return injector<joint_concepts<mpl::vector< Args0> > >(
            args0
        );
    }

    template< typename Args0 , typename Args1>
    explicit injector( Args0 args0 , Args1 args1)
        : detail::module<typename joint_concepts<>::type>( args0 , args1)
    { }

    template< typename Args0 , typename Args1>
    injector<joint_concepts<mpl::vector< Args0 , Args1> > >
    operator()( Args0 args0 , Args1 args1) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1> > >(
            args0 , args1
        );
    }

    template< typename Args0 , typename Args1 , typename Args2>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2)
    { }

    template< typename Args0 , typename Args1 , typename Args2>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2> > >(
            args0 , args1 , args2
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3> > >(
            args0 , args1 , args2 , args3
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> > >(
            args0 , args1 , args2 , args3 , args4
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> > >(
            args0 , args1 , args2 , args3 , args4 , args5
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18
        );
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    explicit injector( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19)
        : detail::module<typename joint_concepts<>::type>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19)
    { }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> > >
    operator()( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19) const {
        return injector<joint_concepts<mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> > >(
            args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19
        );
    }
    };

    }
    }

#endif

