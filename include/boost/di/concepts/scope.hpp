//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPE_HPP
#define BOOST_DI_CONCEPTS_SCOPE_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/type_traits/is_same_base_of.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
namespace concepts {

template<
    typename TScope
  , template<
        typename
      , typename T
      , typename = T
      , typename TName = void
      , typename = ::boost::mpl::vector0<>
      , typename = typename ::boost::mpl::lambda<
            ::boost::mpl::and_<
                ::boost::di::type_traits::is_same_base_of<T, ::boost::mpl::_1>
              , ::boost::is_same<TName, ::boost::mpl::_2>
            >
        >::type
    > class TDependency
>
class scope
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(context)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(concept_type)

    template<typename T, typename U>
    struct rebind
        : T::template rebind<U>::other
    { };

    template<
        typename TExpected
      , typename TGiven = TExpected
      , typename TName = void
      , typename TContext = mpl::vector0<>
    >
    struct dependency
        : TDependency<
               mpl::_1
             , TExpected
             , TGiven
             , TName
             , TContext
          >
    { };

    template<typename T>
    struct is_dependency
        : has_context<T>
    { };

    template<typename T, typename TConcept>
    struct make_dependency
        : rebind<
              dependency<
                  T
                , T
                , typename TConcept::name
                , typename TConcept::call_stack
              >
            , typename TConcept::scope
          >
    { };

    template<
        typename TDeps
      , typename TName = void
      , typename TCallStack = mpl::vector0<>
      , typename T = TScope
    >
    struct concept
    {
        typedef TDeps deps;
        typedef TName name;
        typedef TCallStack call_stack;
        typedef T scope;
    };

    template<typename T>
    struct is_concept
        : has_concept_type<T>
    { };

    template<typename T, typename U>
    struct make_concept
        : concept<
              typename T::concept_type::deps
            , typename T::concept_type::name
            , typename T::concept_type::call_stack
            , typename rebind<U, typename T::concept_type::scope>::type
          >
    { };

    template<typename TConcept>
    struct get_dependencies
        : mpl::fold<
              typename TConcept::deps
            , mpl::vector0<>
            , mpl::copy<
                  mpl::if_<
                      is_dependency<mpl::_2>
                    , mpl::vector1<rebind<mpl::_2, typename TConcept::scope> >
                    , mpl::if_<
                          is_concept<mpl::_2>
                        , get_dependencies<make_concept<mpl::_2, TScope> >
                        , mpl::vector1<make_dependency<mpl::_2, TConcept> >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >::type
    {
        typedef TConcept concept_type;
    };

public:
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct bind
        : get_dependencies<concept<mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> > >
    { };

    template<typename TExpected>
    struct bind<TExpected, BOOST_DI_TYPES_MPL_NA(1)>
        : get_dependencies<concept<mpl::vector<TExpected> > >
    {
        template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
        struct in_call
            : get_dependencies<
                  concept<
                      mpl::vector<TExpected>
                    , void
                    , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                  >
              >
        {
            template<typename TName>
            struct in_name
                : get_dependencies<
                      concept<
                          mpl::vector<TExpected>
                        , TName
                        , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                      >
                  >
            { };
        };

        template<typename TName>
        struct in_name
            : get_dependencies<
                  concept<
                      mpl::vector<TExpected>
                    , TName
                  >
              >
        {
            template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
            struct in_call
                : get_dependencies<
                      concept<
                          mpl::vector<TExpected>
                        , TName
                        , mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                      >
                  >
            { };
        };
    };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

