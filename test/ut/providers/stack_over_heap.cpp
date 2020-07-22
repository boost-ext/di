//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/providers/stack_over_heap.hpp"
#include <memory>
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace providers {

template <class T, class TInit, class... TArgs>
auto test_heap(const aux::type<T>&, const TInit& init, const TArgs&... args) {
  std::unique_ptr<T> object{stack_over_heap{}.get<T>(init, type_traits::heap{}, args...)};
  expect(object.get());
}

template <class T, class TInit, class... TArgs>
auto test_stack(const aux::type<T>&, const TInit& init, const TArgs&... args) {
  T object = stack_over_heap{}.get<T>(init, type_traits::stack{}, args...);
  (void)object;
}

test get_no_args = [] {
  struct c {};
  test_stack(aux::type<int>{}, type_traits::direct{});
  test_heap(aux::type<int>{}, type_traits::uniform{});
  test_stack(aux::type<int>{}, type_traits::uniform{});
  test_heap(aux::type<c>{}, type_traits::direct{});
  test_stack(aux::type<c>{}, type_traits::direct{});
  test_heap(aux::type<c>{}, type_traits::uniform{});
  test_stack(aux::type<c>{}, type_traits::uniform{});
};

test get_with_args = [] {
  struct direct {
    direct(int, double) {}
  };
  struct uniform {
    int i = 0;
    double d = 0.0;
  };
  test_stack(aux::type<direct>{}, type_traits::direct{}, int{}, double{});
  test_heap(aux::type<direct>{}, type_traits::uniform{}, int{}, double{});
#if !defined(__MSVC__)
  test_heap(aux::type<uniform>{}, type_traits::uniform{}, int{}, double{});
#endif
};

}  // namespace providers
