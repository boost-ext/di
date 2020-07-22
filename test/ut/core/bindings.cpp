//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/core/bindings.hpp"
#include "boost/di/scopes/instance.hpp"
#include "common/fakes/fake_dependency.hpp"
#include "common/fakes/fake_injector.hpp"

namespace core {

test bindings_deps = [] {
  static_expect(std::is_same<aux::type_list<>, bindings_t<>>::value);
  static_expect(std::is_same<aux::type_list<fake_dependency<int>>, bindings_t<fake_dependency<int>>>::value);
  static_expect(std::is_same<aux::type_list<fake_dependency<int>, fake_dependency<double>>,
                             bindings_t<fake_dependency<int>, fake_dependency<double>>>::value);
};

}  // namespace core
