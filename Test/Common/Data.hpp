//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSEstring<'1'> _0.txt or copy at http://www.boost.org/LICENSEstring<'1'> _0.txt)
//
#ifndef QDEPS_TEST_COMMON_DATA_HPP
#define QDEPS_TEST_COMMON_DATA_HPP

#include <boost/shared_ptr.hpp>
#include <boost/mpl/string.hpp>
#include "QDeps/Front/Inject.hpp"
#include "QDeps/Utility/Named.hpp"

namespace QDeps
{
namespace Test
{
namespace Common
{

struct If0
{
    virtual ~If0() { }
    virtual void dummy() = 0;
};

struct CIf0 : If0
{
    virtual void dummy() { }
};

struct CIf01 : If0
{
    virtual void dummy() { }
};

struct CIf02 : If0
{
    virtual void dummy() { }
};

struct C0
{
    //trivial ctor
};

struct C1
{
    explicit C1(int = 0) { }
};

struct C2
{
    QDEPS_INJECT(C2, int i, double d, char c)
        : i(i), d(d), c(c)
    { }

    int i;
    double d;
    char c;
};

struct C3
{
    QDEPS_INJECT(explicit C3, int i = 0)
        : i(i)
    { }

    int i;
};

struct C4
{
    QDEPS_INJECT(C4, boost::shared_ptr<C3> c3, Utility::Named<int, boost::mpl::string<'1'> > i1, Utility::Named<int, boost::mpl::string<'2'> > i2)
        : c3(c3), i1(i1), i2(i2)
    { }

    boost::shared_ptr<C3> c3;
    int i1;
    int i2;
};

struct C5
{
    QDEPS_INJECT(C5, boost::shared_ptr<If0> if0, boost::shared_ptr<C2> c2, boost::shared_ptr<C1> c1)
        : if0(if0), c2(c2), c1(c1)
    { }

    boost::shared_ptr<If0> if0;
    boost::shared_ptr<C2> c2;
    boost::shared_ptr<C1> c1;
};

struct C6
{
    QDEPS_INJECT(C6, boost::shared_ptr<C3> c3, const boost::shared_ptr<C4>& c4, C5 c5)
        : c3(c3), c4(c4), c5(c5)
    { }

    boost::shared_ptr<C3> c3;
    boost::shared_ptr<C4> c4;
    C5 c5;
};

struct C7
{
    QDEPS_INJECT(C7, boost::shared_ptr<If0> if0, boost::shared_ptr<C6> c6)
        : if0(if0), c6(c6)
    { }

    boost::shared_ptr<If0> if0;
    boost::shared_ptr<C6> c6;
};

struct C8
{
    QDEPS_INJECT(C8, boost::shared_ptr<C7> c7, C0 c0, boost::shared_ptr<C1> c1, int i)
        : c7(c7), c0(c0), c1(c1), i(i)
    { }

    boost::shared_ptr<C7> c7;
    C0 c0;
    boost::shared_ptr<C1> c1;
    int i;
};

struct C9 : C2
{
    QDEPS_INJECT(C9, int i, double d, char c, std::string s = "string")
        : C2(i, d, c), s(s)
    { }

    std::string s;
};

} // namespace Common
} // namespace Test
} // namespace QDeps

#endif

