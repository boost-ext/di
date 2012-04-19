//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_EXAMPLE_COMMON_DATA_HPP
#define BOOST_DI_EXAMPLE_COMMON_DATA_HPP

#include <boost/shared_ptr.hpp>
#include <boost/mpl/string.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

struct if0
{
    virtual ~if0() { }
    virtual void dummy() = 0;
};

struct c0if0 : if0
{
    virtual void dummy() { }
};

struct c1if0 : if0
{
    virtual void dummy() { }
};

struct c2if0 : if0
{
    virtual void dummy() { }
};

struct c3if0 : if0
{
    BOOST_DI_CTOR(c3if0, int i, double d)
        : i(i), d(d)
    { }

    virtual void dummy() { }

    int i;
    double d;
};

struct c0
{
    //trivial ctor
};

struct c1
{
    explicit c1(int = 0) { }
};

struct c2
{
    BOOST_DI_CTOR(c2, int i, double d, char c)
        : i(i), d(d), c(c)
    { }

    int i;
    double d;
    char c;
};

struct c3
{
    BOOST_DI_CTOR(explicit c3, int i = 0)
        : i(i)
    { }

    int i;
};

struct c4
{
    BOOST_DI_CTOR(c4, boost::shared_ptr<c3> c3_, di::named<int, mpl::string<'1'> > i1, di::named<int, mpl::string<'2'> > i2)
        : c3_(c3_), i1(i1), i2(i2)
    { }

    boost::shared_ptr<c3> c3_;
    int i1;
    int i2;
};

struct c5
{
    BOOST_DI_CTOR(c5, boost::shared_ptr<if0> if0_, boost::shared_ptr<c2> c2_, boost::shared_ptr<c1> c1_)
        : if0_(if0_), c2_(c2_), c1_(c1_)
    { }

    boost::shared_ptr<if0> if0_;
    boost::shared_ptr<c2> c2_;
    boost::shared_ptr<c1> c1_;
};

struct c6
{
    BOOST_DI_CTOR(c6, boost::shared_ptr<c3> c3_, const boost::shared_ptr<c4>& c4_, c5 c5_)
        : c3_(c3_), c4_(c4_), c5_(c5_)
    { }

    boost::shared_ptr<c3> c3_;
    boost::shared_ptr<c4> c4_;
    c5 c5_;
};

struct c7
{
    BOOST_DI_CTOR(c7, boost::shared_ptr<if0> if0_, boost::shared_ptr<c6> c6_)
        : if0_(if0_), c6_(c6_)
    { }

    boost::shared_ptr<if0> if0_;
    boost::shared_ptr<c6> c6_;
};

struct c8
{
    BOOST_DI_CTOR(c8, boost::shared_ptr<c7> c7_, c0 c0_, boost::shared_ptr<c1> c1_, int i)
        : c7_(c7_), c0_(c0_), c1_(c1_), i(i)
    { }

    boost::shared_ptr<c7> c7_;
    c0 c0_;
    boost::shared_ptr<c1> c1_;
    int i;
};

#endif

