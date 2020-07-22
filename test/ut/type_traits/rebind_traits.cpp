//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/rebind_traits.hpp"
#include <memory>
#include <type_traits>
#include "boost/di/aux_/type_traits.hpp"
#include "common/common.hpp"

namespace type_traits {

struct c {};

test traits = [] {
  static_expect(std::is_same<int, rebind_traits_t<_, int>>{});
  static_expect(std::is_same<c, rebind_traits_t<_, c>>{});
  static_expect(std::is_same<named<void, int>, rebind_traits_t<int, named<void>>>{});
  static_expect(std::is_same<std::unique_ptr<int, deleter<int>>, rebind_traits_t<std::unique_ptr<_, deleter<int>>, int>>{});
  static_expect(std::is_same<std::shared_ptr<int>, rebind_traits_t<std::shared_ptr<_>, int>>{});
  static_expect(std::is_same<named<void, std::shared_ptr<int>>, rebind_traits_t<std::shared_ptr<int>, named<void>>>{});
  static_expect(std::is_same<boost::shared_ptr<int>, rebind_traits_t<boost::shared_ptr<_>, int>>{});
  static_expect(std::is_same<named<void, boost::shared_ptr<int>>, rebind_traits_t<boost::shared_ptr<int>, named<void>>>{});
  static_expect(std::is_same<std::weak_ptr<int>, rebind_traits_t<std::weak_ptr<_>, int>>{});
  static_expect(std::is_same<named<void, std::weak_ptr<int>>, rebind_traits_t<std::weak_ptr<int>, named<void>>>{});
};

}  // namespace type_traits
