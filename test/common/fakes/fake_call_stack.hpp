//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_CALL_STACK_HPP
#define BOOST_DI_FAKE_CALL_STACK_HPP

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
#include <boost/mpl/empty.hpp>

namespace boost {
namespace di {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
struct fake_call_stack
{
    typedef aux::mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> context_type;

    template<typename TContext, typename TCallStack>
    struct equal
      : aux::mpl::equal<
            aux::mpl::iterator_range<
                typename aux::mpl::advance<
                    typename aux::mpl::begin<TCallStack>::type
                  , typename aux::mpl::max<
                        aux::mpl::int_<0>
                      , aux::mpl::minus<
                            aux::mpl::size<TCallStack>
                          , aux::mpl::size<TContext>
                        >
                    >::type
                >::type
              , typename aux::mpl::end<TCallStack>::type
            >
          , TContext
        >
    { };

    template<typename TContext, typename TCallStack>
    struct apply_impl
        : aux::mpl::if_<
              aux::mpl::empty<TCallStack>
            , aux::mpl::int_<0>
            , aux::mpl::if_<
                  equal<TContext, TCallStack>
                , aux::mpl::size<TContext>
                , aux::mpl::int_<0>
              >
          >
    { };

    template<typename T>
    struct apply
        : apply_impl<context_type, typename T::call_stack>
    { };

};

} // namespace di
} // namespace boost

#endif

