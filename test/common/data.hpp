//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DATA_HPP
#define BOOST_DI_DATA_HPP

#include <memory>
#include <string>
#include <vector>
#include <cassert>
#include <boost/function.hpp>
#include <boost/mpl/string.hpp>
#include "boost/di/aux_/memory.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/named.hpp"
#include "boost/di/provider.hpp"

namespace boost {
namespace di {

class a { };
class b { };
class c { };
class d { };

struct i
{
    virtual ~i() { }
    virtual void dummy() = 0;
};

struct impl : public i
{
    BOOST_DI_INJECT(impl, int i)
        : i(i)
    { }

    void dummy() override { }

    int i = 0;
};

struct fake : public i
{
    void dummy() override { }
};

struct if0
{
    virtual ~if0() { }
    virtual void dummy() = 0;
};

struct c0if0 : if0
{
    void dummy() override { }
};

struct c1if0 : if0
{
    void dummy() override { }
};

struct c2if0 : if0
{
    void dummy() override { }
};

struct c3if0 : if0
{
    BOOST_DI_INJECT(c3if0
        , int i
        , double d
    )
        : i(i), d(d)
    { }

    void dummy() override { }

    int i = 0;
    double d = 0.0;
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
    BOOST_DI_INJECT(c2
        , int i
        , double d
        , char c
    )
        : i(i), d(d), c(c)
    { }

    int i = 0;
    double d = 0.0;
    char c = 0;
};

struct c3
{
    BOOST_DI_INJECT(explicit c3, int i = 0)
        : i(i)
    { }

    int i = 0;
};

struct c4
{
    BOOST_DI_INJECT(c4
        , aux::shared_ptr<c3> c3_
        , named<int, mpl::string<'1'>> i1
        , named<int, mpl::string<'2'>> i2
    )
        : c3_(c3_), i1(i1), i2(i2)
    { }

    aux::shared_ptr<c3> c3_;
    int i1 = 0;
    int i2 = 0;
};

struct c5
{
    BOOST_DI_INJECT(c5
        , aux::shared_ptr<if0> if0_
        , aux::shared_ptr<c2> c2_
        , aux::shared_ptr<c1> c1_
    )
        : if0_(if0_), c2_(c2_), c1_(c1_)
    { }

    aux::shared_ptr<if0> if0_;
    aux::shared_ptr<c2> c2_;
    aux::shared_ptr<c1> c1_;
};

struct c6
{
    BOOST_DI_INJECT(c6
        , aux::shared_ptr<c3> c3_
        , aux::shared_ptr<c4> c4_
        , c5 c5_
    )
        : c3_(c3_), c4_(c4_), c5_(c5_)
    { }

    aux::shared_ptr<c3> c3_;
    aux::shared_ptr<c4> c4_;
    c5 c5_;
};

struct c7
{
    BOOST_DI_INJECT(c7
        , aux::shared_ptr<if0> if0_
        , aux::shared_ptr<c6> c6_
    )
        : if0_(if0_), c6_(c6_)
    { }

    aux::shared_ptr<if0> if0_;
    aux::shared_ptr<c6> c6_;
};

struct c8
{
    BOOST_DI_INJECT(c8
        , aux::shared_ptr<c7> c7_
        , c0 c0_
        , aux::shared_ptr<c1> c1_
        , int i
    )
        : c7_(c7_), c0_(c0_), c1_(c1_), i(i)
    { }

    aux::shared_ptr<c7> c7_;
    c0 c0_;
    aux::shared_ptr<c1> c1_;
    int i = 0;
};

struct c9 : c2
{
    BOOST_DI_INJECT(c9
        , int i
        , double d
        , char c
        , std::string s = "string"
    )
        : c2(i, d, c), s(s)
    { }

    std::string s;
};

struct c10
{
    BOOST_DI_INJECT_TRAITS(
        named<int, mpl::string<'1'>>
      , named<int, mpl::string<'2'>>
    );

    c10(int i1, int i2)
        : i1(i1), i2(i2)
    { }

    int i1 = 0;
    int i2 = 0;
};

struct c11
{
    BOOST_DI_INJECT(c11
        , named<aux::shared_ptr<int>, mpl::string<'1'>> i
    )
        : i(i)
    { }

    aux::shared_ptr<int> i;
};

struct c12
{
    BOOST_DI_INJECT(c12
        , named<aux::shared_ptr<if0>, mpl::string<'1'>> if0_
        , named<aux::shared_ptr<c2>, mpl::string<'2'>> c2_
    )
        : if0_(if0_), c2_(c2_)
    { }

    aux::shared_ptr<if0> if0_;
    aux::shared_ptr<c2> c2_;
};

struct c13
{
    BOOST_DI_INJECT(c13
        , named<aux::shared_ptr<if0>> if0_
        , c3 c3_
    )
        : if0_(if0_), c3_(c3_)
    { }

    aux::shared_ptr<if0> if0_;
    c3 c3_;
};

struct c14
{
    c14(int i, double d)
        : i(i), d(d)
    { }

    int i = 0;
    double d = 0.0;
};

struct c15
{
    BOOST_DI_INJECT(c15
        , aux::shared_ptr<c3> c3_
        , c6 c6_
    )
        : c3_(c3_), c6_(c6_)
    { }

    aux::shared_ptr<c3> c3_;
    c6 c6_;
};

struct c16
{
    BOOST_DI_INJECT(c16
        , c14& c14_
        , const c3& c3_
    )
        : c14_(c14_), c3_(c3_)
    { }

    c16& operator=(const c16&);

    c14& c14_;
    const c3& c3_;
};

struct c17
{
    BOOST_DI_INJECT(c17
        , const c3& c3_
        , function<int()> f_
        , const std::string& s_
    )
        : c3_(c3_), f_(f_), s_(s_)
    { }

    c3 c3_;
    function<int()> f_;
    std::string s_;
};

struct c18
{
    BOOST_DI_INJECT(c18
        , c0 c0_ // unique
        , aux::shared_ptr<c1> c1_ // shared
        , c3& c3_ // external
    )
        : c0_(c0_), c1_(c1_), c3_(c3_)
    { }

    c18& operator=(const c18&);

    c0 c0_;
    aux::shared_ptr<c1> c1_;
    c3& c3_;
};

struct c19
{
    BOOST_DI_INJECT(c19
        , aux::shared_ptr<c1> c1_
        , aux::shared_ptr<c1> c1__
    )
        : c1_(c1_), c1__(c1__)
    { }

    aux::shared_ptr<c1> c1_;
    aux::shared_ptr<c1> c1__;
};

struct c20
{
    BOOST_DI_INJECT(c20
        , aux::shared_ptr<if0> if0_
        , aux::shared_ptr<if0> if0__
    )
        : if0_(if0_), if0__(if0__)
    { }

    aux::shared_ptr<if0> if0_;
    aux::shared_ptr<if0> if0__;
};

struct c21
{
    BOOST_DI_INJECT(c21
        , aux::shared_ptr<if0> if0_
        , aux::unique_ptr<if0> if0__
    )
        : if0_(if0_), if0__(std::move(if0__))
    { }

    aux::shared_ptr<if0> if0_;
    aux::unique_ptr<if0> if0__;
};

struct c22
{
    BOOST_DI_INJECT(c22
        , const named<aux::shared_ptr<int>, mpl::string<'1'>>& i
        , aux::shared_ptr<c11> c11_
    )
        : i(i), c11_(c11_)
    { }

    aux::shared_ptr<int> i;
    aux::shared_ptr<c11> c11_;
};

struct c23
{
    BOOST_DI_INJECT(c23
        , aux::shared_ptr<if0> if0_
    )
        : if0_(if0_)
    { }

    aux::shared_ptr<if0> if0_;
};

struct c24
{
    BOOST_DI_INJECT(c24
        , c2
        , aux::shared_ptr<int>
        , double*
        , const std::string&
    ) { }
};

struct c25
{
    BOOST_DI_INJECT(c25
        , aux::shared_ptr<int> s1_
        , aux::weak_ptr<int> w1_
    )
        : s1_(s1_), w1_(w1_)
    { }

    aux::shared_ptr<int> s1_;
    aux::weak_ptr<int> w1_;
};

struct c26
{
    BOOST_DI_INJECT(c26
        , std::vector<aux::shared_ptr<if0>> v1_
        , std::vector<int> v2_
    )
        : v1_(v1_), v2_(v2_)
    { }

    std::vector<aux::shared_ptr<if0>> v1_;
    std::vector<int> v2_;
};

struct c27
{
    BOOST_DI_INJECT(c27, aux::shared_ptr<double> d)
        : d_(d)
    { }

    aux::shared_ptr<double> d_;
};

struct c28
{
    c28(int i, double d, float f)
        : i_(i), d_(d), f_(f)
    { }

    BOOST_DI_INJECT_TRAITS(int);

    c28(int i)
        : i_(i)
    { }

    int i_ = 0;
    double d_ = 0.0;
    float f_ = 0.0;
};

struct ref_sp_int
{
    BOOST_DI_INJECT(ref_sp_int, const aux::shared_ptr<int>& i)
        : i_(i)
    {
        assert(i);
    }

    aux::shared_ptr<int> i_;
};

class noncopyable
{
    noncopyable(const noncopyable&);

public:
    BOOST_DI_INJECT(explicit noncopyable, int i = 0)
        : i_(i)
    { }

    int i_ = 0;
};

struct noncopyable_const_ref
{
    BOOST_DI_INJECT(noncopyable_const_ref, const noncopyable& n)
        : i_(n.i_)
    { }

    int i_ = 0;
};

enum eid { e0 = 1 , e1 = 2 };

struct cd2;
struct cd5;

struct cd1
{
    BOOST_DI_INJECT(cd1, cd2*) { };
};

struct cd2
{
    BOOST_DI_INJECT(cd2, cd1*) { };
};

struct cd3
{
    BOOST_DI_INJECT(cd3, cd5*) { };
};

struct cd4
{
    BOOST_DI_INJECT(cd4, cd3*) { };
};

struct cd5
{
    BOOST_DI_INJECT(cd5, cd4*) { };
};

struct ub1
{
    BOOST_DI_INJECT(ub1, int i_)
        : i_(i_)
    { };

    int i_ = 0;
};

struct ub2
{
    BOOST_DI_INJECT(ub2, aux::shared_ptr<ub1> ub1_)
        : ub1_(ub1_)
    { }

    aux::shared_ptr<ub1> ub1_;
};

struct ub3
{
    BOOST_DI_INJECT(ub3, aux::shared_ptr<ub1> ub1_, aux::shared_ptr<ub2> ub2_)
        : ub1_(ub1_), ub2_(ub2_)
    { }

    aux::shared_ptr<ub1> ub1_;
    aux::shared_ptr<ub2> ub2_;
};

struct transaction
{
    transaction(int i)
        : i(i)
    { }

    int i = 0;
};

struct transaction_provider
    : provider<aux::shared_ptr<transaction>>
{
    BOOST_DI_INJECT(transaction_provider
        , aux::shared_ptr<c3> c3_
    )
        : c3_(c3_)
    { }

    aux::shared_ptr<transaction> get() const override
    {
        return aux::shared_ptr<transaction>(new transaction(c3_->i));
    }

    aux::shared_ptr<c3> c3_;
};

struct transaction_usage
{
    BOOST_DI_INJECT(transaction_usage
        , aux::shared_ptr<provider<aux::shared_ptr<transaction>>> p
    )
        : p(p)
    { }

    aux::shared_ptr<provider<aux::shared_ptr<transaction>>> p;
};

template<>
struct ctor_traits<c14>
{
    BOOST_DI_INJECT_TRAITS(int, double);
};

} // namespace di
} // namespace boost

#endif

