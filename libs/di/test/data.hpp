//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_COMMON_DATA_HPP
#define BOOST_DI_TEST_COMMON_DATA_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/string.hpp>
#include "boost/di/ctor.hpp"
#include "boost/di/named.hpp"
#include "boost/di/provider.hpp"

namespace boost {
namespace di {
namespace test {
namespace common {

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
    BOOST_DI_CTOR(c4, shared_ptr<c3> c3_, named<int, mpl::string<'1'> > i1, named<int, mpl::string<'2'> > i2)
        : c3_(c3_), i1(i1), i2(i2)
    { }

    shared_ptr<c3> c3_;
    int i1;
    int i2;
};

struct c5
{
    BOOST_DI_CTOR(c5, shared_ptr<if0> if0, shared_ptr<c2> c2_, shared_ptr<c1> c1_)
        : if0(if0), c2_(c2_), c1_(c1_)
    { }

    shared_ptr<if0> if0;
    shared_ptr<c2> c2_;
    shared_ptr<c1> c1_;
};

struct c6
{
    BOOST_DI_CTOR(c6, shared_ptr<c3> c3_, const shared_ptr<c4>& c4_, c5 c5_)
        : c3_(c3_), c4_(c4_), c5_(c5_)
    { }

    shared_ptr<c3> c3_;
    shared_ptr<c4> c4_;
    c5 c5_;
};

struct c7
{
    BOOST_DI_CTOR(c7, shared_ptr<if0> if0, shared_ptr<c6> c6_)
        : if0(if0), c6_(c6_)
    { }

    shared_ptr<if0> if0;
    shared_ptr<c6> c6_;
};

struct c8
{
    BOOST_DI_CTOR(c8, shared_ptr<c7> c7_, c0 c0_, shared_ptr<c1> c1_, int i)
        : c7_(c7_), c0_(c0_), c1_(c1_), i(i)
    { }

    shared_ptr<c7> c7_;
    c0 c0_;
    shared_ptr<c1> c1_;
    int i;
};

struct c9 : c2
{
    BOOST_DI_CTOR(c9, int i, double d, char c, std::string s = "string")
        : c2(i, d, c), s(s)
    { }

    std::string s;
};

struct c10
{
    BOOST_DI_CTOR_TRAITS(named<int, mpl::string<'1'> >, named<int, mpl::string<'2'> >);

    c10(int i1, int i2)
        : i1(i1), i2(i2)
    { }

    int i1;
    int i2;
};

struct c11
{
    BOOST_DI_CTOR(c11, named< shared_ptr<int>, mpl::string<'1'> > i)
        : i(i)
    { }

    shared_ptr<int> i;
};

struct c12
{
    BOOST_DI_CTOR(c12, named< shared_ptr<if0>, mpl::string<'1'> > if0, named< shared_ptr<c2>, _2> c2_)
        : if0(if0), c2_(c2_)
    { }

    shared_ptr<if0> if0;
    shared_ptr<c2> c2_;
};

struct c13
{
    BOOST_DI_CTOR(c13, named< shared_ptr<if0> > if0, c3 c3_)
        : if0(if0), c3_(c3_)
    { }

    shared_ptr<if0> if0;
    c3 c3_;
};

struct c14_
{
    c14_(int i, double d)
        : i(i), d(d)
    { }

    int i;
    double d;
};

struct cd2;
struct cd5;

struct cd1
{
    BOOST_DI_CTOR(cd1, cd2*) { };
};

struct cd2
{
    BOOST_DI_CTOR(cd2, cd1*) { };
};

struct cd3
{
    BOOST_DI_CTOR(cd3, cd5*) { };
};

struct cd4
{
    BOOST_DI_CTOR(cd4, cd3*) { };
};

struct cd5
{
    BOOST_DI_CTOR(cd5, cd4*) { };
};

struct transaction
{
    transaction(int i)
        : i(i)
    { }

    int i;
};

struct transaction_provider : provider< shared_ptr<transaction> >
{
    BOOST_DI_CTOR(transaction_provider, shared_ptr<c3> c3_)
        : c3_(c3_)
    { }

    virtual shared_ptr<transaction> get() const
    {
        return make_shared<transaction>(c3_->i);
    }

    shared_ptr<c3> c3_;
};

struct transaction_usage
{
    BOOST_DI_CTOR(transaction_usage, shared_ptr< provider< shared_ptr<transaction> > > p)
        : p(p)
    { }

    shared_ptr< provider< shared_ptr<transaction> > > p;
};

} // namespace common
} // namespace test

template<>
struct ctor_traits<test::common::c14>
{
    static void ctor(int, double);
};

} // namespace di
} // namespace boost

#endif

