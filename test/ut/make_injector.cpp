//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/make_injector.hpp"
#include <type_traits>
#include "common/fakes/fake_config.hpp"
#include "common/fakes/fake_dependency.hpp"

test make_injector_empty = [] {
  using injector = decltype(make_injector());
  static_expect(std::is_same<aux::type_list<>, injector::deps>{});
  static_expect(std::is_same<BOOST_DI_CFG, injector::config>{});
};

test make_injector_deps = [] {
  using dep1 = fake_dependency<int>;
  using dep2 = fake_dependency<double>;
  using injector = decltype(make_injector(dep1{}, dep2{}));
  static_expect(std::is_same<aux::type_list<dep1, dep2>, injector::deps>{});
  static_expect(std::is_same<BOOST_DI_CFG, injector::config>{});
};

test make_injector_config_deps = [] {
  using dep = fake_dependency<int>;
  using injector = decltype(make_injector<fake_config<>>(dep{}));
  static_expect(std::is_same<aux::type_list<dep>, injector::deps>{});
  static_expect(std::is_same<fake_config<>, injector::config>{});
};
