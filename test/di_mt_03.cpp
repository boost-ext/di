//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di.hpp"

#include <boost/test/unit_test.hpp>

namespace di = boost::di;

namespace {

struct i
{
    virtual ~i() { }
    virtual void dummy() = 0;
};

struct impl : i
{
    virtual void dummy() { }
};

struct c1
{
    BOOST_DI_CTOR(c1, int i_)
        : i_(i_)
    { }

    int i_;
};

struct c2
{
    BOOST_DI_CTOR(c2
        , boost::shared_ptr<c1> c1_
        , std::auto_ptr<i> p_)
      : c1_(c1_), p_(p_)
    { }

    boost::shared_ptr<c1> c1_;
    std::auto_ptr<i> p_;
};

struct c3
{
    BOOST_DI_CTOR(c3
        , boost::shared_ptr<c1> c1_
        , boost::shared_ptr<c2> c2_)
      : c1_(c1_), c2_(c2_)
    { }

    boost::shared_ptr<c1> c1_;
    boost::shared_ptr<c2> c2_;
};

} // namespace

BOOST_AUTO_TEST_CASE(create) {
    const int i = 42;

    typedef di::injector<
        impl
      , di::singleton<c1>
    > injector_base;

    BOOST_AUTO(inj, make_injector(
        injector_base()
      , di::bind<int>::to(i)
      , di::per_request<c2>()
    ));

    boost::shared_ptr<c3> c3_ = inj.create<boost::shared_ptr<c3> >();

    BOOST_CHECK(c3_->c1_ == c3_->c2_->c1_);
    BOOST_CHECK_EQUAL(i, c3_->c1_->i_);
    BOOST_CHECK_EQUAL(i, c3_->c2_->c1_->i_);
    BOOST_CHECK(dynamic_cast<impl*>(c3_->c2_->p_.get()));
}

