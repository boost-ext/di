//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct I {
  virtual ~I() noexcept = default;
  virtual void dummy() = 0;
};
struct Impl : I {
  void dummy() override {}
};

auto module = [] {
  // clang-format off
  return di::make_injector(
    // di::bind<I>.to<Impl>()
  );
  // clang-format on
};

int main() {
  auto injector = di::make_injector(module());
  std::unique_ptr<I> object{injector.create<I *>()};
  object->dummy();
}

// clang-format off
/**
 * di.cpp:32:38: warning: 'create<I *, 0>' is deprecated: creatable constraint not satisfied
 *   std::unique_ptr<I> object{injector.create<I *>()};
 *                                      ^
 * boost/di.hpp:2420:3: note: 'create<I *, 0>' has been explicitly marked deprecated here
 *   create
 *   ^
 * boost/di.hpp:871:2: error: 'boost::di::v1_0_0::concepts::abstract_type<I>::is_not_bound::error'
 *  error(_ = "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?");
 *  ^
 */
// clang-format on
