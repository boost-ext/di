//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_CALL_STACK_HPP
#define BOOST_DI_BINDINGS_CALL_STACK_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/type_traits/make_plain.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/max.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/transform.hpp>

namespace boost {
namespace di {
namespace bindings {

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class call_stack
{
    typedef BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T) context_type;

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

public:
    template<typename T>
    struct apply
        : apply_impl<
              context_type
            , typename aux::mpl::transform<
                  typename T::call_stack
                , di::type_traits::make_plain<aux::mpl::_>
              >::type
          >::type
    { };
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

