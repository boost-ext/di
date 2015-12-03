//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[msm
//<-
#include <cassert>
#include <iostream>
#include "msm.hpp"

//->
struct e1 {};
struct e2 {};
struct e3 {};

auto guard1 = [](auto, int i) {
  assert(42 == i);
  return true;
};
auto guard2 = [](auto) { return true; };
auto action1 = [](auto) {};
auto action2 = [](auto) {};

auto controller() noexcept {
  struct config {
    static void no_transition() noexcept {}
  };

  using namespace msm;
  auto idle = init_state<__COUNTER__>{};
  auto idle2 = init_state<__COUNTER__>{};
  auto s1 = state<__COUNTER__>{};
  auto s2 = state<__COUNTER__>{};

  // clang-format off
  return make_transition_table<config>(
   // +-----------------------------------------------------------------+
      idle    == s1 + event<e1> [guard1] / (action1, action2)
   // +-----------------------------------------------------------------+
	, idle2   == s2 + event<e2> [guard1 && guard2] / action1
   // +-----------------------------------------------------------------+
  );
  // clang-format on
}

int main() {
  auto injector = di::make_injector(di::bind<int>().to(42));
  auto sm = injector.create<decltype(controller())>();

  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
  sm.process_event(e2{});
  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
  sm.process_event(e2{});
  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
  sm.process_event(e3{});
  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
}

//]
