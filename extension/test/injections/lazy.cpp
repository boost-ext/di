//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/injections/lazy.hpp"

#include <cassert>
#include <memory>

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy1() = 0;
};

struct type_expensive_to_create : interface {
  void dummy1() override {}
};

/*<<example `example` class>>*/
class example {
 public:
  explicit example(di::extension::lazy<std::unique_ptr<interface>> l) : l(l) {}

  void initialize() {
    auto object = l.get();
    assert(dynamic_cast<type_expensive_to_create *>(object.get()));
  }

 private:
  di::extension::lazy<std::unique_ptr<interface>> l;
};

int main() {
  {
    /*<<define injector>>*/
    // clang-format off
    auto injector = di::make_injector(
      di::bind<interface>().to<type_expensive_to_create>()
    );
    // clang-format on

    /*<<create `example`>>*/
    auto object = injector.create<example>();
    object.initialize();
  }
#if defined(__EXCEPTIONS)
  {
    /*<<define injector>>*/
    // clang-format off
    auto injector = di::make_injector(
        di::bind<int>().to([]()->int{ throw 0; })
    );
    auto exception_thrown = false;
    try {
        auto lazy = injector.create<di::extension::lazy<int>>();
        /*<<call throws>>*/
        lazy.get();
    }
    catch (...) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    // clang-format on
  }
#endif
}
