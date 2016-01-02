//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[multiple_interfaces
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface1 {
  virtual ~interface1() noexcept = default;
};
struct interface2 {
  virtual ~interface2() noexcept = default;
};
struct implementation : interface1, interface2 {};
//->

class multiple_interfaces {
 public:
  multiple_interfaces(const std::shared_ptr<interface1>& interface1_, const std::shared_ptr<interface2>& interface2_) {
    assert(dynamic_cast<implementation*>(interface1_.get()));
    assert(dynamic_cast<implementation*>(interface2_.get()));
    assert(static_cast<implementation*>(interface1_.get()) == static_cast<implementation*>(interface2_.get()));
  }
};

int main() {
  /*<<create injector with binding `implementation` to `interface1` and `interface2` using `di::any_of`>>*/
  auto injector = di::make_injector(di::bind<interface1, interface2>().to<implementation>());

  injector.create<multiple_interfaces>();
}

//]
