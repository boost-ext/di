//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/unique.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "common/fakes/fake_injector.hpp"
#include "common/fakes/fake_provider.hpp"
#include "common/fakes/fake_scope.hpp"

namespace scopes {

test create_deduce = [] {
  int i = exposed<>::scope<int, int>{fake_injector<int>{}}.create<int>(fake_provider<int>{});
  expect(0 == i);
};

test create_fake = [] {
  int i = exposed<fake_scope<>>::scope<int, int>{fake_injector<int>{}}.create<int>(fake_provider<int>{});
  expect(0 == i);
};

test create_unique = [] {
  int i = exposed<scopes::unique>::scope<int, int>{fake_injector<int>{}}.create<int>(fake_provider<int>{});
  expect(0 == i);
};

test create_singleton = [] {
  std::shared_ptr<int> i =
      exposed<scopes::singleton>::scope<int, int>{fake_injector<int*>{}}.create<int>(fake_provider<int>{});
  expect(!i);
};

}  // scopes
