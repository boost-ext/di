//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_TEST_COMMON_INTERFACES_HPP
#define QDEPS_TEST_COMMON_INTERFACES_HPP

namespace QDeps
{
namespace Test
{
namespace Common
{

template<int = 0> class I
{
public:
    virtual ~I() { };
    virtual int get() const = 0;
};

class ILevel
{
public:
    virtual ~ILevel() { };
    virtual int getLevel() const = 0;
    virtual int get() const = 0;
};

} // namespace Common
} // namespace Test
} // namespace QDeps

#endif

