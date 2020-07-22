//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/unique.hpp"
#include <memory>
#include "common/fakes/fake_provider.hpp"

namespace scopes {

test create_unique = [] {
  unique::scope<int, int> unique;
  std::unique_ptr<int> object1 = unique.create<std::unique_ptr<int>, no_name>(fake_provider<int>{});
  std::unique_ptr<int> object2 = unique.create<std::unique_ptr<int>, no_name>(fake_provider<int>{});
  expect(object1 != object2);
};

test create_shared_but_unique = [] {
  unique::scope<int, int> unique;
  std::shared_ptr<int> object1 = unique.create<std::shared_ptr<int>, no_name>(fake_provider<int>{});
  std::shared_ptr<int> object2 = unique.create<std::shared_ptr<int>, no_name>(fake_provider<int>{});
  expect(object1 != object2);
};

}  // namespace scopes
