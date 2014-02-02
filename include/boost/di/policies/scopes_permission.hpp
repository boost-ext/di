//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_SCOPES_PERMISSION_HPP
#define BOOST_DI_POLICIES_SCOPES_PERMISSION_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/detail/binder.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/assert.hpp>

namespace boost {
namespace di {
namespace policies {

template<typename TScope>
struct allow_scope
{
    template<typename T>
    struct allow
        : is_same<T, TScope>
    { };
};

/**
 * @code
 * scopes_permission<>
 * bind<deduce<int>> // error
 *
 * scopes_permission<allow_scope<scopes::unique>>
 * bind<unique<int>> // ok
 *
 * @endcode
 */
template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class scopes_permission
{
    template<
        typename T
      , typename TCallStackSize
      , typename TCtor =
            typename type_traits::ctor_traits<
                typename type_traits::make_plain<T>::type
            >::type
    >
    struct ctor
        : mpl::if_<
              mpl::and_<
                  mpl::empty<TCtor>
                , mpl::equal_to<TCallStackSize, mpl::int_<1> >
              >
            , mpl::vector1<T>
            , TCtor
          >::type
    { };

    template<
        typename T
      , typename TCallStack
      , typename TBind
    >
    struct binder
        : TBind::template get_dependency<T, TCallStack>::type
    { };

    template<typename TAllow, typename T>
    struct is_allowed_impl
        : TAllow::template allow<T>
    { };

    template<
        typename T
      , typename TAllows = mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
    >
    struct is_allowed
        : mpl::bool_<
              mpl::count_if<
                  TAllows
                , is_allowed_impl<mpl::_, T>
              >::value != 0
          >
    { };

    template<typename T>
    struct scope
    {
        typedef typename T::scope type;
    };

    template<
        typename T
      , typename TBind
      , typename TCallStack =
            mpl::vector1<typename type_traits::make_plain<T>::type>
    >
    struct scopes_permission_impl
        : mpl::fold<
              ctor<typename binder<T, TCallStack, TBind>::given, mpl::size<TCallStack> >
            , mpl::vector0<>
            , mpl::copy<
                  mpl::joint_view<
                      mpl::if_<
                          is_allowed<
                              scope<binder<mpl::_2, TCallStack, TBind> >
                          >
                        , mpl::_2 // ignore
                        , mpl::vector1<
                              mpl::pair<
                                  mpl::_2
                                , scope<binder<mpl::_2, TCallStack, TBind> >
                              >
                          >
                      >
                    , scopes_permission_impl<
                          mpl::_2
                        , TBind
                        , mpl::push_back<
                              TCallStack
                            , type_traits::make_plain<mpl::_2>
                          >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >
    { };

    template<typename T, typename TAssert>
    struct verify_impl : T
    {
        BOOST_MPL_ASSERT_MSG(
            !TAssert::value || mpl::empty<T>::value
          , SCOPE_NOT_PERMITTED
          , (T)
        );
    };

public:
    template<
        typename TDeps
      , typename TGiven
      , typename TAssert = mpl::true_
      , template<typename> class TBinder = di::detail::binder
    >
    struct verify
        : verify_impl<
              typename scopes_permission_impl<TGiven, TBinder<TDeps> >::type
            , TAssert
          >
    { };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

