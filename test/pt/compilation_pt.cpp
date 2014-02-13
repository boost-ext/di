//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>

#include "boost/di.hpp"

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
    explicit c1(int i_ = 0, double d_ = 0.0)
        : i_(i_), d_(d_)
    { }

    int i_ = 0;
    double d_ = 0.0;
};

struct c2
{
    c2(std::shared_ptr<c1> c1_
     , std::unique_ptr<i> p_)
      : c1_(c1_), p_(std::move(p_))
    { }

    std::shared_ptr<c1> c1_;
    std::unique_ptr<i> p_;
};

struct c3
{
    c3(std::shared_ptr<c1> c1_
     , std::shared_ptr<c2> c2_
     , c1 c1__)
      : c1_(c1_), c2_(c2_), c1__(c1__)
    { }

    std::shared_ptr<c1> c1_;
    std::shared_ptr<c2> c2_;
    c1 c1__;
};

} // namespace

int main() {

    const int i_ = 42;
    const double d_ = 42.0;

    using injector_c0 = di::injector<
        di::bind_int<i_>
      , impl
    >;

    auto injector_ = make_injector(
        injector_c0()
      , di::unique<c2>()
      , di::bind<double>::to(d_)
    );

    auto c3_ = injector_.create<std::shared_ptr<c3>>();

    assert(dynamic_cast<impl*>(c3_->c2_->p_.get()));
    assert(c3_->c1_.get() == c3_->c2_->c1_.get());
    assert(i_ == c3_->c1_->i_);
    assert(d_ == c3_->c1_->d_);
    assert(i_ == c3_->c1__.i_);
    assert(d_ == c3_->c1__.d_);
    assert(i_ == c3_->c2_->c1_->i_);
    assert(d_ == c3_->c2_->c1_->d_);

	return 0;
}

