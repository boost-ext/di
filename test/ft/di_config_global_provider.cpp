//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_DI_CFG_FWD class global_provider;
#define BOOST_DI_CFG global_provider
#include <boost/di.hpp>
#include <utility>

namespace di = boost::di;

auto called = 0;

class custom_provider {
 public:
  template <class...>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class T, class TInitialization, class TMemory, class... TArgs>
  auto get(const TInitialization&, const TMemory&, TArgs&&... args) const {
    ++called;
    return new T(std::forward<TArgs>(args)...);
  }
};

class global_provider : public di::config {
 public:
  static auto provider(...) noexcept { return custom_provider{}; }
};

test call_provider_via_global_config = [] {
  called = 0;
  auto injector = di::make_injector();
  injector.create<int>();
  expect(1 == called);
};
