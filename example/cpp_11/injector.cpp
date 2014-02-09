//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <typeinfo>
#include <iostream>
#include <memory>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct name { };

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { c0(di::named<int, name>, double) { } };
struct c01 : i0 { };
struct c1 { c1(std::shared_ptr<i0>) { } };
struct c2 { c2(std::shared_ptr<c1>, int, double, char) { } };
struct c3 { };
struct c4 { };
struct c5
{
    c5(std::shared_ptr<c1>
     , c2
     , std::shared_ptr<i0>
     , std::shared_ptr<c3>
     , c4&
    ) { }
};

} // namespace

int main() {
    std::shared_ptr<c3> c3_(new c3);
    c4 c4_;

    auto injector = di::make_injector(
        di::shared<
            c1
        >()
      , di::unique<
            c0
        >()
      , di::scope<di::scopes::unique<>>::bind<
            di::bind_int<42>
        >()
      , di::unique<
            di::bind_int<87>::named<name>
          , di::bind<c01>::when<di::call_stack<c2, c1>>
        >()
      , di::bind<double>::to(42.0)
      , di::bind<double>::when<c0>::to(87.0)
      , di::bind<c3>::to(c3_)
      , di::bind<c4>::to(c4_)
    );

    injector.create<c5>();
}

