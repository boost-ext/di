//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_BINDER_HPP
#define BOOST_DI_DETAIL_BINDER_HPP

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/scope_traits.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"
#include "boost/di/concepts/dependency.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/times.hpp>

namespace boost {
namespace di {
namespace detail {

template<typename TDeps = mpl::vector0<> >
class binder
{
    template<typename TDependency, typename T, typename TCallStack>
    struct comparator
        : mpl::times<typename mpl::apply<typename TDependency::bind, T, TCallStack>::type, mpl::times<mpl::int_<100>, mpl::plus<mpl::int_<1>, typename TDependency::scope::priority> > >
    { };

public:
    template<typename T, typename TCallStack
  , typename TDefault =
        ::boost::di::concepts::dependency<
            typename type_traits::scope_traits<T>::type
          , typename type_traits::make_plain<T>::type
        >
    >
    struct resolve
        : mpl::deref<
        mpl::second<typename mpl::fold<
                TDeps
                , mpl::pair<mpl::int_<0>, TDefault>
                , mpl::if_<
                    mpl::greater<
                    comparator<
                      mpl::_2
                    , T
                    , TCallStack
                  >
                  , mpl::first<mpl::_1>
                  >
                 , mpl::pair<comparator<mpl::_2, T, TCallStack>, mpl::_2>
                 , mpl::_1
                >
          >::type
        > >::type::template rebind<typename type_traits::scope_traits<T>::type>::other
    { };
};

} // namespace detail
} // namespace di
} // namespace boost

#endif

