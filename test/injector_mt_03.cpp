//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/typeof/typeof.hpp"
#include "boost/di/injector.hpp"
#include "boost/di/make_injector.hpp"
#include "boost/di/ctor.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

namespace {

struct i { virtual ~i() { } virtual void dummy() = 0; };
struct impl : i { virtual void dummy() { } };
struct c1 { BOOST_DI_CTOR(c1, int i_) : i_(i_) { } int i_; };
struct c2 { BOOST_DI_CTOR(c2, shared_ptr<c1> c1_, std::auto_ptr<i> i_) : c1_(c1_), i_(i_) { } shared_ptr<c1> c1_; std::auto_ptr<i> i_; };
struct c3 { BOOST_DI_CTOR(c3, shared_ptr<c1> c1_, shared_ptr<c2> c2_) : c1_(c1_), c2_(c2_) { } shared_ptr<c1> c1_; shared_ptr<c2> c2_; };

//struct c4 { BOOST_DI_CTOR(c4, std::auto_ptr<i> i_) {}};
} // namespace

BOOST_AUTO_TEST_CASE(create) {
    const int i = 42;

    typedef injector<
        impl
      , singleton<c1>
    > injector_base;

    BOOST_AUTO(inj, make_injector(
        injector_base()
      , bind<int>::to(i)
      , per_request<c2>()
    ));

    //shared_ptr<c3> c3_ = inj.create<shared_ptr<c3> >();
    //inj.create<shared_ptr<c4> >();
}

} // namespace di
} // namespace boost

