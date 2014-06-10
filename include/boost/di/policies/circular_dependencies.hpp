//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP
#define BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP

#include "boost/di/aux_/config.hpp"

#include <boost/mpl/count.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/size.hpp>

namespace boost {
namespace di {
namespace policies {

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

public:
    template<typename TDependency>
    void assert_policy() const {
       BOOST_DI_ASSERT_MSG(
            is_unique_call_stack<typename TDependency::call_stack>::value
          , CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED
          , typename TDependency::call_stack
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

