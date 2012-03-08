//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_FRONT_FUSION_AUX_BIND_HPP
#define QDEPS_FRONT_FUSION_AUX_BIND_HPP

namespace QDeps
{
namespace Front
{
namespace Fusion
{

template<typename T> struct Bind
{
    template<typename TImpl> struct To
    {
        template<typename TScope> struct InScope
        {
            template<typename TCall> struct InCall
            {
            };
        };

        template<typename TCall> struct InCall
        {
        };
    };
};

} // namespace Fusion
} // namespace Front
} // namespace QDeps

#endif

