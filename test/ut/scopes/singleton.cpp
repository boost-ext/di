//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/singleton.hpp"
#include <memory>
#include "common/fakes/fake_provider.hpp"

namespace scopes {

test create_singleton = [] {
  singleton::scope<int, int> singleton;
  std::shared_ptr<int> object1 = singleton.create<int, no_name>(fake_provider<int>{});
  std::shared_ptr<int> object2 = singleton.create<int, no_name>(fake_provider<int>{});
  expect(object1 == object2);
};

struct c {};
auto has_shared_ptr__(c &&) -> std::false_type;

test create_singleton_from_ptr_to_ptr = [] {
  singleton::scope<c, c> singleton;
  c& object1 = singleton.create<c, no_name>(fake_provider<c>{});
  c& object2 = singleton.create<c, no_name>(fake_provider<c>{});
  expect(&object1 == &object2);
};

test create_singleton_from_ptr_to_const_ptr = [] {
  singleton::scope<c, c> singleton;
  const c& object1 = singleton.create<c, no_name>(fake_provider<c>{});
  const c& object2 = singleton.create<c, no_name>(fake_provider<c>{});
  expect(&object1 == &object2);
};

}  // namespace scopes
