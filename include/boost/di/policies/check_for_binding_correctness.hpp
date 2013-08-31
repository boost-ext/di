//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CHECK_FOR_BINDING_CORRECTNESS_HPP
#define BOOST_DI_POLICIES_CHECK_FOR_BINDING_CORRECTNESS_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/assert.hpp>

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost {
namespace di {
namespace policies {

class check_for_binding_correctness
{
public:
    template<
        typename TDeps
      , typename
      , bool Assert = true
      , template<
            typename
          , typename = void
        > class TCtorTraits = type_traits::ctor_traits
    >
    class verify
    {
        template<typename T>
        struct ctor
            : mpl::transform<
                  TCtorTraits<T>
                , type_traits::make_plain<mpl::_1>
              >::type
        { };

        template<typename T>
        struct expected
        {
            typedef typename T::expected type;
        };

        template<typename T>
        struct context
            : mpl::fold<
                  typename T::context
                , mpl::vector0<>
                , mpl::if_<
                      mpl::is_sequence<mpl::_2>
                    , mpl::push_back<
                          mpl::_1
                        , mpl::back<mpl::_2>
                      >
                    , mpl::push_back<mpl::_1, mpl::_2>
                  >
              >::type
        { };

        template<
            typename TCtor
          , typename TDependency
        >
        struct comparator
            : mpl::count_if<
                  TCtor
                , typename TDependency::bind
              >::type
        { };

        template<
            typename TDependency
          , typename TResult
        >
        struct for_each_context
            : mpl::fold<
                  context<TDependency>
                , TResult
                , mpl::if_<
                      comparator<
                          ctor<mpl::_2>
                        , TDependency
                      >
                    , mpl::_1
                    , mpl::push_back<
                          mpl::_1
                        , mpl::pair<
                              typename expected<TDependency>::type
                            , mpl::_2
                          >
                      >
                  >
              >::type
        { };

        struct binding_impl
            : mpl::fold<
                  TDeps
                , mpl::vector0<>
                , for_each_context<
                      mpl::_2
                    , mpl::_1
                  >
              >::type
        { };

        template<typename T = typename binding_impl::type>
        struct binding
            : mpl::empty<T>
        {
           BOOST_MPL_ASSERT_MSG(
                !Assert || (Assert && mpl::empty<T>::value)
              , BINDING_DECLARATION_IS_NOT_CORRECT
              , (T)
            );
        };

    public:
        typedef typename binding<>::type type;
    };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

