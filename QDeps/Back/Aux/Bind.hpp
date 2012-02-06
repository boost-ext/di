//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_BIND_HPP
#define QDEPS_BACK_AUX_BIND_HPP

#include <boost/mpl/vector.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{

template
<
    typename TScope,
    typename T,
    typename TDeps = boost::mpl::vector0<>
>
class Bind
{ };

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

