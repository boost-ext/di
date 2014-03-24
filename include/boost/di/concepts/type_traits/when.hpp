//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_WHEN_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_WHEN_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/empty.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

template<typename TContext>
class when
{
    template<
        typename TBind
      , typename T
      , typename TCallStack
      , typename TScope
    >
    struct apply_bind
        : TBind::template apply<T, TCallStack, TScope>::type
    { };

public:
    template<
        typename T
      , typename TCallStack
      , typename TScope
    >
    struct apply
        : mpl::if_<
              mpl::empty<TContext>
            , mpl::int_<1>
            , typename mpl::deref<
                  mpl::max_element<
                      mpl::transform_view<
                          TContext
                        , apply_bind<mpl::_1, T, TCallStack, TScope>
                      >
                  >
              >::type
          >::type
    { };
};

} // namespace type_traits
} // namespace concepts
} // namespace di
} // namespace boost

#endif

