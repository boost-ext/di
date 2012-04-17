//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_UTILITY_PROVIDER_HPP
#define DI_UTILITY_PROVIDER_HPP

namespace di
{
namespace Utility
{

template<typename T>
class Provider
{
public:
    virtual ~Provider() { }
    virtual T get() const = 0;
};

} // namespace Utility
} // namespace di

#endif

