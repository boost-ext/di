//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CHECK_FOR_CIRCULAR_DEPENDENCIES_HPP
#define BOOST_DI_POLICIES_CHECK_FOR_CIRCULAR_DEPENDENCIES_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/assert.hpp>

#include "boost/di/config.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/detail/binder.hpp"

namespace boost {
namespace di {
namespace policies {

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

/**
 *@code
 * struct cd2;
 * struct cd1 { cd1(cd2*); };
 * struct cd2 { cd2(cd1*); };
 * make_injector().create<cd1>();
 *@endcode
 */
class check_for_circular_dependencies
{
public:
    template<
        typename TDeps
      , typename TGiven
      , bool Assert = true
      , template<typename> class TBinder = detail::binder
    >
    class verify
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
          , typename TCallStack =
                mpl::vector1<typename type_traits::make_plain<T>::type>
          , typename = void
        >
        struct deps
            : mpl::fold<
                  ctor<typename binder<T, TCallStack, TBind>::given>
                , mpl::true_
                , mpl::and_<
                      mpl::_1
                    , deps<
                          mpl::_2
                        , TBind
                        , mpl::push_back<
                              TCallStack
                            , type_traits::make_plain<mpl::_2>
                          >
                      >
                  >
              >::type
        { };

        template<
            typename T
          , typename TBind
          , typename TCallStack
        >
        struct deps<
            T
          , TBind
          , TCallStack
          , typename disable_if<is_unique_call_stack<TCallStack> >::type
        >
            : mpl::false_
        {
            BOOST_MPL_ASSERT_MSG(
                !Assert
              , CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED
              , (T, TCallStack)
            );
        };

    public:
        typedef typename deps<TGiven, TBinder<TDeps> >::type type;
    };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

