//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(BOOST_DI_PREPROCESSED)
	#include "boost/di/aux_/preprocessed/concepts/type_traits/is_req_call.hpp"
#endif

#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQ_CALL_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQ_CALL_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/max.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/empty.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

template<typename TContext, typename TCallStack>
struct equal_call_stack
    : mpl::equal<
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
{ };

template<typename TContext, typename TCallStack>
struct get_longest_call_stack_size
    : mpl::fold<
          TContext
        , mpl::int_<0>
        , mpl::if_<
             equal_call_stack<mpl::_2, TCallStack>
            , mpl::size<mpl::_2>
            , mpl::_1
          >
      >::type
{ };

template<typename TContext, typename TCallStack = mpl::_2>
struct is_req_call
    : mpl::if_<
            mpl::empty<TContext>
        , mpl::int_<1>
         , mpl::times<mpl::int_<10>, get_longest_call_stack_size<TContext, TCallStack> >
      >::type
{ };

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

#endif

