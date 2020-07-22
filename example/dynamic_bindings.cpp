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

//<-
enum eid { e1 = 1, e2 = 2 };
struct interface {
  virtual ~interface() noexcept = default;
};
struct implementation1 : interface {};
struct implementation2 : interface {};
//->

/*<<module configuration>>*/
auto dynamic_bindings = [](eid& id) {
  return di::make_injector(
      /*<<bind `interface` to lazy lambda expression>>*/
      di::bind<interface>().to([&](const auto& injector) -> std::shared_ptr<interface> {
        switch (id) {
          default:
            return nullptr;
          case e1:
            return injector.template create<std::shared_ptr<implementation1>>();
          case e2:
            return injector.template create<std::shared_ptr<implementation2>>();
        }

        return nullptr;
      }));
};

int main() {
  auto id = e1;

  /*<<create interface with `id = e1`>>*/
  auto injector = di::make_injector(dynamic_bindings(id));
  assert(dynamic_cast<implementation1*>(injector.create<std::shared_ptr<interface>>().get()));

  id = e2;
  /*<<create interface with `id = e2`>>*/
  assert(dynamic_cast<implementation2*>(injector.create<std::shared_ptr<interface>>().get()));
  (void)id;
}
