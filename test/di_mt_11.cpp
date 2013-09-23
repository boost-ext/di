//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <boost/shared_ptr.hpp>
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
    BOOST_DI_CTOR(explicit c1, int i_ = 0, double d_ = 0.0)
        : i_(i_), d_(d_)
    { }

    int i_ = 0;
    double d_ = 0.0;
};

struct c2
{
    BOOST_DI_CTOR(c2
        , std::shared_ptr<c1> c1_
        , std::unique_ptr<i> p_)
      : c1_(c1_), p_(std::move(p_))
    { }

    std::shared_ptr<c1> c1_;
    std::unique_ptr<i> p_;
};

struct c3
{
    BOOST_DI_CTOR(c3
        , boost::shared_ptr<c1> c1_
        , boost::shared_ptr<c2> c2_
        , c1 c1__)
      : c1_(c1_),c2_(c2_), c1__(c1__)
    { }

    boost::shared_ptr<c1> c1_;
    boost::shared_ptr<c2> c2_;
    c1 c1__;
};

} // namespace

BOOST_AUTO_TEST_CASE(create_complex) {
    const int i = 42;
    const double d = 42.0;

    using injector_c0 = di::injector<
        di::policy<
            di::policies::check_for_binding_correctness
        >
      , impl
    >;

    auto injector_c1 = di::make_injector(
        di::policy<
            di::policies::check_for_circular_dependencies
        >()
      , di::bind_int<i>()
    );

    auto injector_ = make_injector(
        injector_c0()
      , di::per_request<c2>()
      , injector_c1
      , di::bind<double>::to(d)
    );

    auto c3_ = injector_.create<boost::shared_ptr<c3>>();

    BOOST_CHECK(dynamic_cast<impl*>(c3_->c2_->p_.get()));
    BOOST_CHECK_EQUAL(c3_->c1_.get(), c3_->c2_->c1_.get());
    BOOST_CHECK_EQUAL(i, c3_->c1_->i_);
    BOOST_CHECK_EQUAL(d, c3_->c1_->d_);
    BOOST_CHECK_EQUAL(i, c3_->c1__.i_);
    BOOST_CHECK_EQUAL(d, c3_->c1__.d_);
    BOOST_CHECK_EQUAL(i, c3_->c2_->c1_->i_);
    BOOST_CHECK_EQUAL(d, c3_->c2_->c1_->d_);
}

