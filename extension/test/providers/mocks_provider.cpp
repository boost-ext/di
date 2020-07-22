//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/providers/mocks_provider.hpp"

#include <cassert>

namespace di = boost::di;

struct i1 {
  virtual ~i1() noexcept = default;
  virtual int get() = 0;
};

struct i2 {
  virtual ~i2() noexcept = default;
  virtual int get() = 0;
};

struct c {
  c(std::shared_ptr<i1> sp, std::unique_ptr<i2> up, int i) {
    assert(sp->get() == 42);
    assert(up->get() == 123);
    assert(i == 87);
  }
};

int main() {
  // clang-format off
  /*<<unit test>>*/
  {
    /*<<create injector with `mocks_provider`>>*/
    auto mi = di::extension::mocks_injector();

    /*<<set expectations>>*/
    expect(mi, &i1::get).will_return(42);
    expect(mi, &i2::get).will_return(123);

    /*<<create object to test with interfaces to be injected by di and int value passed directly to constructor>>*/
    c object{mi, mi, 87};
  }

  /*<<integration test>>*/
  {
    struct impl1 : i1 {
      int get() override { return 42; }
    };

    /*<<create injector with `mocks_provider`>>*/
    // clang-format off
      auto mi = di::extension::mocks_injector(
        di::bind<int>().to(87) // custom value
      , di::bind<i1>().to<impl1>() // original implementation
      );
    // clang-format on

    /*<<set expectations>>*/
    expect(mi, &i2::get).will_return(123);  // fake

    /*<<create object to test with mocked `i1` and original `i2` and injected int value>>*/
    mi.create<c>();
  }
  // clang-format on
}
