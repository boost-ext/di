//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <initializer_list>  // has to be before, due to the bug in clang < 3.7
//->
#include <boost/di.hpp>
//<-
#include <cassert>
#include <memory>
#include <set>
#include <vector>
//->

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation1 : interface {
  void dummy() override {}
};
struct implementation2 : interface {
  void dummy() override {}
};
//->

struct example {
  example(std::vector<std::unique_ptr<interface>> v, std::vector<int> i) {
    assert(v.size() == 4);
    assert(dynamic_cast<implementation1*>(v[0].get()));
    assert(dynamic_cast<implementation2*>(v[1].get()));
    assert(dynamic_cast<implementation1*>(v[2].get()));
    assert(dynamic_cast<implementation2*>(v[3].get()));

    assert(i.size() == 7);
    assert(i[0] == 1);
    assert(i[1] == 2);
    assert(i[2] == 3);
    assert(i[3] == 5);
    assert(i[4] == 8);
    assert(i[5] == 13);
    assert(i[6] == 21);
  }
};

int main() {
  // clang-format off
  auto il = {1, 2, 3, 5, 8, 13, 21};
  auto injector = di::make_injector(
    di::bind<int[]>().to(il)
  , di::bind<interface* []>().to<implementation1, implementation2, interface, di::named<class Implementation2>>()
  , di::bind<interface>().to<implementation1>()  // <------------------/                         |
  , di::bind<interface>().named<class Implementation2>().to<implementation2>()  // <-------------/
  );
  // clang-format on

  injector.create<example>();

  assert(injector.create<std::vector<int>>().size() == 7);
  assert(injector.create<std::set<int>>().size() == 7);

  assert(injector.create<std::vector<std::shared_ptr<interface>>>().size() == 4);
  assert(injector.create<std::set<std::shared_ptr<interface>>>().size() == 2);  // unique instances
}
