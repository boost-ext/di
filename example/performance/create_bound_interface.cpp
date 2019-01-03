//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cstdlib>
#include <memory>
#include <string>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation : interface {
  void dummy() override {}
};
//->

auto test() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<interface>().to<implementation>()
  );
  // clang-format on

  return injector.create<std::unique_ptr<interface>>();
}

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
 * retq
 */

//<-
int main(int, char** argv) {
  std::system(("gdb -batch -ex 'file " + std::string{argv[0]} + "' -ex 'disassemble test'").c_str());
}
//->
