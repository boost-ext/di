//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/extension/providers/heap.hpp"

#include <cassert>
#include <memory>

namespace di = boost::di;

class new_call;
class del_call;

struct example {
  template <class>
  static auto &calls() {
    static auto i = 0;
    return i;
  }

  void *operator new(size_t size) {
    ++calls<new_call>();
    return ::operator new(size);
  }

  void operator delete(void *ptr) {
    ++calls<del_call>();
    ::operator delete(ptr);
  }
};

int main() {
  struct heap_config : di::config {
    auto provider(...) const noexcept { return di::extension::heap{}; }
  };

  auto injector = di::make_injector<heap_config>();

  assert(0 == example::calls<new_call>());
  assert(0 == example::calls<del_call>());
  injector.create<example>();
  assert(1 == example::calls<new_call>());
  assert(1 == example::calls<del_call>());
}
