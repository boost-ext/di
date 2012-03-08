//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_UTILITY_PROVIDER_HPP
#define QDEPS_UTILITY_PROVIDER_HPP

namespace QDeps
{
namespace Utility
{

template<typename T>
class Provider
{
public:
    virtual T get() = 0;
};

} // namespace Utility
} // namespace QDeps

#endif

