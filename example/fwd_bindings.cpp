//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
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

class interface;
class implementation;

auto configuration = [] {
  // clang-format off
  return di::make_injector(
    /*<<binding using fwd declarations, no checking whether types are related*/
    di::bind<interface>().to<implementation>()
  );
  // clang-format on
};

/*<<binding using fwd declarations, no checking whether types are related*/
class interface {
 public:
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
class implementation : public interface {
 public:
  void dummy() override {}
};

int main() {
  /*<<make injector configuration>>*/
  auto injector = configuration();
  assert(dynamic_cast<implementation*>(injector.create<std::unique_ptr<interface>>().get()));
}
