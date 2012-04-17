//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DI_BACK_POLICIES_CHECK_FOR_CREATION_OWNERSHIP_HPP
#define DI_BACK_POLICIES_CHECK_FOR_CREATION_OWNERSHIP_HPP

#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>
#include "di/config.hpp"

namespace di
{
namespace back
{
namespace Policies
{

class CheckForCreationOwnership
{
public:
    template<typename TDeps, typename TGiven, typename Enable = void>
    class Assert : public boost::mpl::true_
    { };

    template<typename TDeps, typename TGiven>
    class Assert<TDeps, TGiven, typename boost::enable_if< boost::is_reference<TGiven> >::type> : public boost::mpl::false_
    {
       DI_STATIC_ASSERT(
            false,
            CREATION_OWNERSHIP_IS_NOT_CLEAR,
            (TGiven)
        );
    };
};

} // namespace Policies
} // namespace back
} // namespace di

#endif

