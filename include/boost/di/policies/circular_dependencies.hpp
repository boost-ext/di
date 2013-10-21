//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP
#define BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP

#include "boost/di/detail/binder.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/assert.hpp>

namespace boost {
namespace di {
namespace policies {

namespace detail {
template<typename TCallStack>
struct is_unique_call_stack
    : mpl::bool_<
          static_cast<std::size_t>(mpl::accumulate<
              typename mpl::transform<
                  TCallStack
                , mpl::count<TCallStack, mpl::_>
              >::type
            , mpl::int_<0>
            , mpl::plus<mpl::_1, mpl::_2>
          >::type::value
        ) == mpl::size<TCallStack>::value
      >
{ };

} // namespace detail

/**
 *@code
 * struct cd2;
 * struct cd1 { cd1(cd2*); };
 * struct cd2 { cd2(cd1*); };
 * make_injector().create<cd1>();
 *@endcode
 */
class circular_dependencies
{
    template<typename T>
    struct ctor
        : type_traits::ctor_traits<
              typename type_traits::make_plain<T>::type
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

    template<
        typename T
      , typename TBind
      , typename TAssert
      , typename TCallStack =
            mpl::vector1<typename type_traits::make_plain<T>::type>
      , typename = void
    >
    struct circular_dependencies_impl
        : mpl::fold<
              ctor<typename binder<T, TCallStack, TBind>::given>
            , mpl::true_
            , mpl::and_<
                  mpl::_1
                , circular_dependencies_impl<
                      mpl::_2
                    , TBind
                    , TAssert
                    , mpl::push_back<
                          TCallStack
                        , type_traits::make_plain<mpl::_2>
                      >
                  >
              >
          >
    { };

    template<
        typename T
      , typename TBind
      , typename TAssert
      , typename TCallStack
    >
    struct circular_dependencies_impl<
        T
      , TBind
      , TAssert
      , TCallStack
      , typename disable_if<detail::is_unique_call_stack<TCallStack> >::type
    >
        : mpl::false_
    {
       BOOST_MPL_ASSERT_MSG(
            !TAssert::value
          , CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED
          , (T, TCallStack)
        );
    };

public:
    typedef circular_dependencies is_policy;

    template<
        typename TDeps
      , typename TGiven
      , typename TAssert = mpl::true_
      , template<typename> class TBinder = di::detail::binder
    >
    struct verify
        : circular_dependencies_impl<TGiven, TBinder<TDeps>, TAssert>
    { };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

