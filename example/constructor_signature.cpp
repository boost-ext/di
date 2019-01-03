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
struct interface1 {
  virtual ~interface1() noexcept = default;
};
struct interface2 {
  virtual ~interface2() noexcept = default;
};
struct implementation1 : interface1 {};
struct implementation2 : interface2 {};
//->

/*<define `example` class as usual>*/
class example {
 public:
  example(std::unique_ptr<interface1> up, std::shared_ptr<interface2> sp, int i) {
    assert(dynamic_cast<implementation1*>(up.get()));
    assert(dynamic_cast<implementation2*>(sp.get()));
    assert(42 == i);
  }
};

/*<define `example` class with different parameters order - won't require any changes in injector configuration>*/
class example_with_different_parameters_order {
 public:
  example_with_different_parameters_order(std::shared_ptr<interface2> sp, int i, std::unique_ptr<interface1> up) {
    assert(dynamic_cast<implementation1*>(up.get()));
    assert(dynamic_cast<implementation2*>(sp.get()));
    assert(42 == i);
  }
};

/*<define `example` class with different parameters order and different types - won't require any changes in injector
 * configuration>*/
class example_with_different_parameters_order_and_types {
 public:
  example_with_different_parameters_order_and_types(std::unique_ptr<interface2> sp, const int& i, interface1* up) : up_(up) {
    assert(dynamic_cast<implementation1*>(up));
    assert(dynamic_cast<implementation2*>(sp.get()));
    assert(42 == i);
  }

  // in order to delete pointer - Boost.DI always transfer ownership to user in case of pointers
  std::shared_ptr<interface1> up_;
};

int main() {
  /*<<make injector>>*/
  // clang-format off
  auto injector = di::make_injector(
    di::bind<interface1>().to<implementation1>()
  , di::bind<interface2>().to<implementation2>()
  , di::bind<int>().to(42)
  );
  // clang-format on

  /*<create different examples using the same injector configuration>*/
  injector.create<example>();
  injector.create<example_with_different_parameters_order>();
  injector.create<example_with_different_parameters_order_and_types>();
}
