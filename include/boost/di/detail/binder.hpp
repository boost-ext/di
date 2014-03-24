//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_BINDER_HPP
#define BOOST_DI_DETAIL_BINDER_HPP

#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/scope_traits.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"
#include "boost/di/convertibles/convertible.hpp"
#include "boost/di/detail/builder.hpp"
#include "boost/di/concepts/dependency.hpp"

#include <boost/mpl/if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/greater.hpp>

namespace boost {
namespace di {
namespace detail {

template<
    typename TDependecies
  , typename TBuilder = builder
>
class binder
{
    template<
        typename TDependency
      , typename T
      , typename TCallStack
    >
    struct apply
        : TDependency::bind::template apply<
              T
            , TCallStack
            , typename TDependency::scope
          >::type
    { };

public:
    explicit binder(TBuilder builder = TBuilder())
        : builder_(builder)
    { }

    template<
        typename T
      , typename TCallStack
      , typename TDefault =
            ::boost::di::concepts::dependency<
                typename type_traits::scope_traits<T>::type
              , typename type_traits::make_plain<T>::type
            >
    >
    struct resolve
        : mpl::deref<
              mpl::second<
                  typename mpl::fold<
                      TDependecies
                    , mpl::pair<mpl::int_<0>, TDefault>
                    , mpl::if_<
                          mpl::greater<
                              apply<
                                  mpl::_2
                                , T
                                , TCallStack
                              >
                            , mpl::first<mpl::_1>
                          >
                        , mpl::pair<apply<mpl::_2, T, TCallStack>, mpl::_2>
                        , mpl::_1
                      >
                  >::type
              >
          >::type::template rebind<
              typename type_traits::scope_traits<T>::type
          >::other
    { };

    template<
        typename T
      , typename TCtor
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TCreatePolicy
      , typename TCreator
      , typename TDeps
      , typename TRefs
      , typename TVisitor
    >
    const convertibles::convertible<T>&
    resolve_impl(TCreator& creator, TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        return builder_.template build<
            T
          , TCtor
          , TCallStack
          , TPolicies
          , TDependency
          , TCreatePolicy
          , TCreator
        >(creator, deps, refs, visitor);
    }

private:
    TBuilder builder_;
};

} // namespace detail
} // namespace di
} // namespace boost

#endif

