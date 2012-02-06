//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_TEST_COMMON_CTORS_HPP
#define QDEPS_TEST_COMMON_CTORS_HPP

#include <boost/shared_ptr.hpp>
#include "QDeps/Front/Ctor.hpp"
#include "Test/Common/Interfaces.hpp"

namespace QDeps
{
namespace Test
{
namespace Common
{

class SharedPtrI
{
public:
    QDEPS_CTOR(SharedPtrI, boost::shared_ptr<I<> > p_i)
        : m_i(p_i)
    { }

    int get() const { return m_i->get(); }

private:
    boost::shared_ptr<I<> > m_i;
};

class ConstRefI
{
public:
    QDEPS_CTOR(ConstRefI, const I<>& p_i)
        : m_i(p_i)
    { }

    int get() const { return m_i.get(); }

private:
    const I<>& m_i;
};

class RefI
{
public:
    QDEPS_CTOR(RefI, I<>& p_i)
        : m_i(p_i)
    { }

    int get() const { return m_i.get(); }

private:
    I<>& m_i;
};

class SharedPtrISharedPtrI
{
public:
    QDEPS_CTOR(SharedPtrISharedPtrI, boost::shared_ptr<I<1> > p_1, boost::shared_ptr<I<2> > p_2)
        : m_1(p_1), m_2(p_2)
    { }

    int get1() const { return m_1->get(); }
    int get2() const { return m_2->get(); }

private:
    boost::shared_ptr<I<1> > m_1;
    boost::shared_ptr<I<2> > m_2;
};

template<int Value>
class C : public I<Value>
{
public:
    virtual int get() const { return Value; }
};

template<int IValue, int LevelValue>
class LevelSecond : public ILevel
{
public:
    QDEPS_CTOR(LevelSecond, boost::shared_ptr<I<IValue> > p_arg)
        : m_arg(p_arg)
    { }

    int get() const { return m_arg->get(); }
    int getLevel() const { return LevelValue; }

private:
    boost::shared_ptr<I<IValue> > m_arg;
};

class LevelFirst
{
public:
    QDEPS_CTOR(LevelFirst, boost::shared_ptr<ILevel> p_arg)
        : m_arg(p_arg)
    { }

    boost::shared_ptr<ILevel> level() const { return m_arg; }

private:
    boost::shared_ptr<ILevel> m_arg;
};

} // namespace Common
} // namespace Test
} // namespace QDeps

#endif

