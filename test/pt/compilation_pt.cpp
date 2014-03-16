//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>
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

    int i_;
    double d_;
};

struct c2
{
    c2(boost::shared_ptr<c1> c1_, boost::shared_ptr<i> p_)
      : c1_(c1_), p_(p_)
    { }

    boost::shared_ptr<c1> c1_;
    boost::shared_ptr<i> p_;
};

struct c3
{
    c3(boost::shared_ptr<c1> c1_
     , boost::shared_ptr<c2> c2_
     , c1 c1__
     , const std::vector<int>& v_)
       : c1_(c1_), c2_(c2_), c1__(c1__), v_(v_)
    { }

    boost::shared_ptr<c1> c1_;
    boost::shared_ptr<c2> c2_;
    c1 c1__;
    std::vector<int> v_;
};

} // namespace

int main() {
    const int i = 42;
    const double d = 42.0;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    typedef di::injector<impl> injector_c0;

    BOOST_AUTO(injector_c1, di::make_injector(
        di::bind_int<i>()
      , di::bind<std::vector<int> >::to(v)
    ));

    BOOST_AUTO(injector_, make_injector(
        injector_c0()
      , di::unique<c2>()
      , injector_c1
      , di::bind<double>::to(d)
    ));

    boost::shared_ptr<c3> c3_ = injector_.create<boost::shared_ptr<c3> >(
        di::policies::creation_ownership()
      , di::policies::circular_dependencies()
      , di::policies::arguments_permission<
            di::policies::allow_smart_ptrs
          , di::policies::allow_copies
          , di::policies::allow_refs
        >()
    );

    assert(dynamic_cast<impl*>(c3_->c2_->p_.get()));
    assert(c3_->c1_.get() == c3_->c2_->c1_.get());
    assert(i == c3_->c1_->i_);
    assert(d == c3_->c1_->d_);
    assert(i == c3_->c1__.i_);
    assert(d == c3_->c1__.d_);
    assert(i == c3_->c2_->c1_->i_);
    assert(d == c3_->c2_->c1_->d_);
    assert(3u == v.size());
    assert(v.size() == c3_->v_.size());
    assert(v[0] == c3_->v_[0]);
    assert(v[1] == c3_->v_[1]);
    assert(v[2] == c3_->v_[2]);
}

