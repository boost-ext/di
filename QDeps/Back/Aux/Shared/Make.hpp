//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_SHARED_MAKE_HPP
#define QDEPS_BACK_AUX_SHARED_MAKE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace Shared
{

namespace Detail
{
BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
} // namespace Detail

template<typename T, typename Enable = void>
struct Make
{
    typedef boost::shared_ptr<T> type;
};

template<typename T>
struct Make<T, typename boost::enable_if< Detail::has_element_type<T> >::type>
{
    typedef T type;
};

} // namespace Shared
} // namespace Back
} // namespace Aux
} // namespace QDeps

#endif

