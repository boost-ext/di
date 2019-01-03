//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
//->
#include <boost/di.hpp>

namespace di = boost::di;

auto name = [] {};

struct ctor_inject {
  /*<<constructor with intrusive named parameter explicitly selected>>*/
  BOOST_DI_INJECT(ctor_inject, int i1, (named = name) int i2) {
    assert(i1 == 0);
    assert(i2 == 42);
  }
};

struct ctor_inject_traits {
  /*<<constructor with less intrusive named parameter using traits>>*/
  BOOST_DI_INJECT_TRAITS(int, (named = name) int);
  explicit ctor_inject_traits(int i1, int i2 = 0) {
    assert(i1 == 0);
    assert(i2 == 42);
  }
};

struct ctor_di_traits {
  /*<<class without any changes>>*/
  explicit ctor_di_traits(int i1, int i2 = 0) {
    assert(i1 == 0);
    assert(i2 == 42);
  }
};

namespace boost {
namespace di {

template <>
struct ctor_traits<ctor_di_traits> {
  /*<<no intrusive way of defining named parameters>>*/
  BOOST_DI_INJECT_TRAITS(int, (named = name) int);
};

}  // namespace di
}  // namespace boost

struct ctor_inject_traits_no_limits {
  /*<<constructor with 20 parameters>>*/
  using boost_di_inject__ =
      di::inject<int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>;

  ctor_inject_traits_no_limits(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int,
                               int) {}
};

int main() {
  /*<<make injector>>*/
  // clang-format off
  auto injector = di::make_injector(
    di::bind<int>().named(name).to(42)
  );
  // clang-format on

  /*<<create dependencies>>*/
  injector.create<ctor_inject>();
  injector.create<ctor_inject_traits>();
  injector.create<ctor_di_traits>();

  injector.create<ctor_inject_traits_no_limits>();
}
