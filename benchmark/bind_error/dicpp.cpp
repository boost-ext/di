//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <di/registry.hpp>
#include <di/injector.hpp>
#include <di/constructor.hpp>

struct I {
  virtual ~I() noexcept = default;
  virtual void dummy() = 0;
};
struct Impl : I {
  DI_CONSTRUCTOR(Impl, ()) {}
  void dummy() override {}
};

void module(di::registry& r) {
  // r.add(r.type<I>().implementation<Impl>());
}

int main() {
  di::injector injector;
  injector.install(module);
  std::unique_ptr<I> object{injector.construct<I*>()};
  object->dummy();
}

/**
 * terminate called after throwing an instance of 'std::runtime_error'
 * what():  Requested type without corresponding provider: di::type_key<I*, void>.
 * Double check if the type is abstract
 */
