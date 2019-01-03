//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#if defined(__cpp_concepts)
//->

#include "boost/di/extension/injections/concepts.hpp"

#include <cassert>

namespace di = boost::di;

template <typename T>
concept bool Dummy() {
  return requires(T a) { {a.dummy()}; };
}

struct DummyImpl {
  void dummy() {}
};

auto dummy_concept = [] {};

struct example {
  BOOST_DI_INJECT(example, int i, auto t, (named = dummy_concept) Dummy d, (named = dummy_concept) std::unique_ptr<Dummy> up) {
    assert(42 == i);
    static_assert(std::is_same<decltype(t), int>::value, "");
    assert(87 == t);
    static_assert(std::is_same<decltype(d), DummyImpl>::value, "");
    static_assert(std::is_same<decltype(up), std::unique_ptr<DummyImpl>>::value, "");
  }
};

int main() {
  // clang-format off
  auto injector = di::make_injector<di::extension::concepts_provider_config>(
      di::bind<int>.to(42)
    , di::bind<di::_>.to(87)
    , di::bind<di::_>().named(dummy_concept).to<DummyImpl>()
  );
  // clang-format on

  injector.create<example>();
}

//<-
#else
int main() {}
#endif
//->
