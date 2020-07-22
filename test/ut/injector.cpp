//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/injector.hpp"
#include <type_traits>
#include "boost/di/scopes/instance.hpp"

test injector_exposed = [] {
  using injector_t = injector<int>;
  using dep = core::dependency<scopes::instance, aux::type_list<int>, aux::type_list<named<no_name, int>>>;
  static_expect(std::is_same<aux::type_list<dep>, injector_t::deps>{});
};

test injector_exposed_many = [] {
  using injector_t = injector<int, double>;
  using dep = core::dependency<scopes::instance, aux::type_list<int, double>,
                               aux::type_list<named<no_name, int>, named<no_name, double>>>;
  static_expect(std::is_same<aux::type_list<dep>, injector_t::deps>{});
};
