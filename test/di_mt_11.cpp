//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <vector>

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
    BOOST_DI_INJECT(explicit c1, int i_ = 0, double d_ = 0.0)
        : i_(i_), d_(d_)
    { }

    int i_ = 0;
    double d_ = 0.0;
};

struct c2
{
    BOOST_DI_INJECT(c2
        , std::shared_ptr<c1> c1_
        , std::unique_ptr<i> p_)
      : c1_(c1_), p_(std::move(p_))
    { }

    std::shared_ptr<c1> c1_;
    std::unique_ptr<i> p_;
};

struct c3
{
    BOOST_DI_INJECT(c3
        , std::shared_ptr<c1> c1_
        , std::shared_ptr<c2> c2_
        , c1 c1__
        , const std::vector<int>& v_)
      : c1_(c1_), c2_(c2_), c1__(c1__), v_(v_)
    { }

    std::shared_ptr<c1> c1_;
    std::shared_ptr<c2> c2_;
    c1 c1__;
    std::vector<int> v_;
};

} // namespace

BOOST_AUTO_TEST_CASE(create_complex) {
    const int i = 42;
    const double d = 42.0;
    const std::vector<int> v = {1, 2, 3};

    using injector_c0 = di::injector<
        di::policies::binding_correctness()
      , impl
    >;

    auto injector_c1 = di::make_injector(
        di::policies::circular_dependencies()
      , di::bind_int<i>()
      , di::bind<std::vector<int>>::to(v)
    );

    auto injector_ = make_injector(
        injector_c0()
      , di::unique<c2>()
      , injector_c1
      , di::bind<double>::to(d)
      , di::policies::arguments_permission<
            di::policies::allow_smart_ptrs
          , di::policies::allow_copies
          , di::policies::allow_refs
        >()
    );

    auto c3_ = injector_.create<std::shared_ptr<c3>>();

    BOOST_CHECK(dynamic_cast<impl*>(c3_->c2_->p_.get()));
    BOOST_CHECK_EQUAL(c3_->c1_.get(), c3_->c2_->c1_.get());
    BOOST_CHECK_EQUAL(i, c3_->c1_->i_);
    BOOST_CHECK_EQUAL(d, c3_->c1_->d_);
    BOOST_CHECK_EQUAL(i, c3_->c1__.i_);
    BOOST_CHECK_EQUAL(d, c3_->c1__.d_);
    BOOST_CHECK_EQUAL(i, c3_->c2_->c1_->i_);
    BOOST_CHECK_EQUAL(d, c3_->c2_->c1_->d_);
    BOOST_CHECK_EQUAL(3u, v.size());
    BOOST_CHECK_EQUAL(v.size(), c3_->v_.size());
    BOOST_CHECK_EQUAL(v[0], c3_->v_[0]);
    BOOST_CHECK_EQUAL(v[1], c3_->v_[1]);
    BOOST_CHECK_EQUAL(v[2], c3_->v_[2]);
}

