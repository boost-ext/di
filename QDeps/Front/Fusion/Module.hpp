//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_FUSION_AUX_MODULE_HPP
#define QDEPS_FRONT_FUSION_AUX_MODULE_HPP

#include <QPool/Pool.hpp>

namespace QDeps
{
namespace Front
{
namespace Fusion
{

class Module
{
public:
    template<typename T1, typename T2, typename T3>
    QPool::Pool< boost::mpl::vector<T1, T2, T3> > operator()(T1, T2, T3)
    {
        return QPool::Pool< boost::mpl::vector<T1, T2, T3> >();
    }
};

} // namespace Fusion
} // namespace Front
} // namespace QDeps

#endif

