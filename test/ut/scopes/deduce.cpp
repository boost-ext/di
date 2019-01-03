//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/scopes/deduce.hpp"
#include "common/fakes/fake_injector.hpp"
#include "common/fakes/fake_provider.hpp"
#include "common/fakes/fake_scope.hpp"

namespace scopes {

struct c {};

struct fake_scope_config {
  template <class>
  struct scope_traits {
    using type = fake_scope<>;
  };
};

test create = [] {
  expect(0 == static_cast<int>(
                  deduce::scope<int, int>{}.create<int, no_name>(fake_provider<int, fake_injector<int>, fake_scope_config>{})));
};

test create_from_scope = [] {
  fake_scope<>::calls() = 0;
  c c_ = deduce::scope<c, c>{}.create<c, no_name>(fake_provider<c, fake_injector<c>, fake_scope_config>{});
  (void)c_;
  expect(1 == fake_scope<>::calls());
};

}  // namespace scopes
