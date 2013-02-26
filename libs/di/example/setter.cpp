//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct i0 { virtual ~i0() { }; };

struct c0 : i0 { };

struct c
{
    c()
        : i_(0), d_(0.0)
    { }

    void BOOST_DI_SETTER(boost::shared_ptr<i0> sp, int i) {
        sp_ = sp;
        i_ = i;
    }

    boost::shared_ptr<i0> sp_;
    int i_;
};

} // namespace

int main()
{
    di::injector<
        bind<int, mpl::int_<42> >
      , c0
    > injector;

    {
        boost::shared_ptr<c> c = injector.create<c>();
        assert(42 == c.i_);
        assert(dynamic_cast<c0&>(*c.sp_));
    }

    return 0;
}

