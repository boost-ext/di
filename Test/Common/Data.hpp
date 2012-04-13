//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_TEST_COMMON_DATA_HPP
#define QDEPS_TEST_COMMON_DATA_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/string.hpp>
#include "QDeps/Front/Ctor.hpp"
#include "QDeps/Utility/Named.hpp"
#include "QDeps/Utility/Provider.hpp"

namespace QDeps
{
namespace Test
{
namespace Common
{

using namespace boost;
using namespace boost::mpl;
using namespace Utility;

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

struct CIf03 : If0
{
    QDEPS_CTOR(CIf03, int i, double d)
        : i(i), d(d)
    { }

    virtual void dummy() { }

    int i;
    double d;
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
    QDEPS_CTOR(C2, int i, double d, char c)
        : i(i), d(d), c(c)
    { }

    int i;
    double d;
    char c;
};

struct C3
{
    QDEPS_CTOR(explicit C3, int i = 0)
        : i(i)
    { }

    int i;
};

struct C4
{
    QDEPS_CTOR(C4, shared_ptr<C3> c3, Named<int, mpl::string<'1'> > i1, Named<int, mpl::string<'2'> > i2)
        : c3(c3), i1(i1), i2(i2)
    { }

    shared_ptr<C3> c3;
    int i1;
    int i2;
};

struct C5
{
    QDEPS_CTOR(C5, shared_ptr<If0> if0, shared_ptr<C2> c2, shared_ptr<C1> c1)
        : if0(if0), c2(c2), c1(c1)
    { }

    shared_ptr<If0> if0;
    shared_ptr<C2> c2;
    shared_ptr<C1> c1;
};

struct C6
{
    QDEPS_CTOR(C6, shared_ptr<C3> c3, const shared_ptr<C4>& c4, C5 c5)
        : c3(c3), c4(c4), c5(c5)
    { }

    shared_ptr<C3> c3;
    shared_ptr<C4> c4;
    C5 c5;
};

struct C7
{
    QDEPS_CTOR(C7, shared_ptr<If0> if0, shared_ptr<C6> c6)
        : if0(if0), c6(c6)
    { }

    shared_ptr<If0> if0;
    shared_ptr<C6> c6;
};

struct C8
{
    QDEPS_CTOR(C8, shared_ptr<C7> c7, C0 c0, shared_ptr<C1> c1, int i)
        : c7(c7), c0(c0), c1(c1), i(i)
    { }

    shared_ptr<C7> c7;
    C0 c0;
    shared_ptr<C1> c1;
    int i;
};

struct C9 : C2
{
    QDEPS_CTOR(C9, int i, double d, char c, std::string s = "string")
        : C2(i, d, c), s(s)
    { }

    std::string s;
};

struct C10
{
    QDEPS_CTOR_TRAITS(Named<int, mpl::string<'1'> >, Named<int, mpl::string<'2'> >);

    C10(int i1, int i2)
        : i1(i1), i2(i2)
    { }

    int i1;
    int i2;
};

struct C11
{
    QDEPS_CTOR(C11, Named< shared_ptr<int>, mpl::string<'1'> > i)
        : i(i)
    { }

    shared_ptr<int> i;
};

struct C12
{
    QDEPS_CTOR(C12, Named< shared_ptr<If0>, mpl::string<'1'> > if0, Named< shared_ptr<C2>, _2> c2)
        : if0(if0), c2(c2)
    { }

    shared_ptr<If0> if0;
    shared_ptr<C2> c2;
};

struct C13
{
    QDEPS_CTOR(C13, Named<C2> c2)
        : c2(c2)
    { }

    C2 c2;
};

struct C14
{
    C14(int i, double d)
        : i(i), d(d)
    { }

    int i;
    double d;
};

struct CD2;
struct CD5;

struct CD1
{
    QDEPS_CTOR(CD1, CD2*) { };
};

struct CD2
{
    QDEPS_CTOR(CD2, CD1*) { };
};

struct CD3
{
    QDEPS_CTOR(CD3, CD5*) { };
};

struct CD4
{
    QDEPS_CTOR(CD4, CD3*) { };
};

struct CD5
{
    QDEPS_CTOR(CD5, CD4*) { };
};

struct Transaction
{
    Transaction(int i)
        : i(i)
    { }

    int i;
};

struct TransactionProvider : Provider< shared_ptr<Transaction> >
{
    QDEPS_CTOR(TransactionProvider, shared_ptr<C3> c3)
        : c3(c3)
    { }

    virtual shared_ptr<Transaction> get() const
    {
        return boost::make_shared<Transaction>(c3->i);
    }

    shared_ptr<C3> c3;
};

struct TransactionUsage
{
    QDEPS_CTOR(TransactionUsage, shared_ptr< Provider< shared_ptr<Transaction> > > p)
        : p(p)
    { }

    shared_ptr< Provider< shared_ptr<Transaction> > > p;
};

} // namespace Common
} // namespace Test

template<>
struct CtorTraits<Test::Common::C14>
{
    static void ctor(int, double);
};

} // namespace QDeps

#endif

