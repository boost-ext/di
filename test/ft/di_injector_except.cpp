//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

namespace di = boost::di;

test should_throw_when_ctor_throws = [] {
  struct except {
    except() { throw 0; }
  };

  const auto injector = di::make_injector();

  auto cought = false;
  try {
    injector.create<except>();
  } catch (...) {
    cought = true;
  }
  expect(cought);
};

struct empty {};

test should_throw_when_lambda_with_2_args_throws = [] {
  const auto injector = di::make_injector(di::bind<empty>().to([](const auto&, const auto&) {
    throw 0;
    return empty{};
  }));

  auto cought = false;
  try {
    injector.create<empty>();
  } catch (...) {
    cought = true;
  }
  expect(cought);
};

test should_throw_when_lambda_with_1_arg_throws = [] {
  const auto injector = di::make_injector(di::bind<empty>().to([](const auto&) {
    throw 0;
    return empty{};
  }));

  auto cought = false;
  try {
    injector.create<empty>();
  } catch (...) {
    cought = true;
  }
  expect(cought);
};

test should_throw_when_lambda_with_no_args_throws = [] {
  const auto injector = di::make_injector(di::bind<empty>().to([]() {
    throw 0;
    return empty{};
  }));

  auto cought = false;
  try {
    injector.create<empty>();
  } catch (...) {
    cought = true;
  }
  expect(cought);
};

struct except_factory {
  template <class TInjector>
  auto operator()(const TInjector&) const {
    throw 0;
    return empty{};
  }
};

test should_throw_when_factory_with_1_arg_throws = [] {
  const auto injector = di::make_injector(di::bind<empty>().to(except_factory{}));

  auto cought = false;
  try {
    injector.create<empty>();
  } catch (...) {
    cought = true;
  }
  expect(cought);
};