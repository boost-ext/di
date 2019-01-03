//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct I {
  virtual ~I() noexcept = default;
  virtual void dummy() = 0;
};
struct Impl : I {
  void dummy() override {}
};

auto module = [] {
  // clang-format off
  return di::make_injector(
    di::bind<I>.to<Impl>()
  );
  // clang-format on
};

auto test() {
  auto injector = di::make_injector(module());
  return injector.create<std::unique_ptr<I>>();
}

//<-
int main() {}
/**
 * ASM x86-64 (same as `std::make_unique<implementation>()`)
 *
 * push   %rbx
 * mov    %rdi,%rbx
 * mov    $0x8,%edi
 * callq  0x4009f0 <_Znwm@plt>
 * movq   $0x400e78,(%rax)
 * mov    %rax,(%rbx)
 * mov    %rbx,%rax
 * pop    %rbx
 * ret
 */
//->
