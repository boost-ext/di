//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_FUSION_AUX_INST_HPP
#define QDEPS_FRONT_FUSION_AUX_INST_HPP

#include <boost/shared_ptr.hpp>

namespace QDeps
{
namespace Front
{
namespace Fusion
{

template<typename T> static double Inst(const boost::shared_ptr<T>&)
{ }

} // namespace Fusion
} // namespace Front
} // namespace QDeps

#endif

