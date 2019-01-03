//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation : interface {
  void dummy() override {}
};
//->

struct dependency1 {
  dependency1(std::shared_ptr<interface> spi /*shared*/
              ,
              const std::shared_ptr<interface>& spi_ /*shared*/)
      : spi_(spi), spi__(spi_) {
    assert(spi.get() == spi_.get());
  }

  std::shared_ptr<interface> spi_;
  std::shared_ptr<interface> spi__;
};

struct dependency2 {
  dependency2(std::shared_ptr<interface> spi /*shared*/, int i /*unique*/) : spi_(spi) { assert(i == 0); }

  std::shared_ptr<interface> spi_;
};

struct example {
  example(std::unique_ptr<dependency1> dependency1_ /*unique*/
          ,
          const dependency2& dependency2_ /*unique temporary*/) {
    assert(dependency2_.spi_.get() == dependency1_->spi_.get());
    assert(dependency2_.spi_.get() == dependency1_->spi__.get());
  }
};

int main() {
  /*<<create injector with deduced `interface`>>*/
  // clang-format off
  auto injector = di::make_injector(
    di::bind<interface>().to<implementation>()  // => di::bind<interface>().to<implementation>().in(di::deduce)
  );
  // clang-format on

  /*<<create `example`>>*/
  injector.create<example>();
}
