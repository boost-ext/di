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

auto module = []() -> di::injector<std::unique_ptr<interface>> {
  // clang-format off
  return di::make_injector(
    di::bind<interface>().to<implementation>()
  );
  // clang-format on
};

auto test() {
  auto injector = di::make_injector(module());
  return injector.create<std::unique_ptr<interface>>();
}

/**
 * ASM x86-64 (additional cost for type erasure)
 *
 * push   %r14
 * push   %rbx
 * push   %rax
 * mov    %rdi,%r14
 * mov    $0x18,%edi
 * callq  0x4008e0 <_Znwm@plt>
 * mov    %rax,%rbx
 * movq   $0x400bb0,(%rbx)
 * movq   $0x400bd0,0x8(%rbx)
 * mov    $0x8,%edi
 * callq  0x4008e0 <_Znwm@plt>
 * movq   $0x400cd8,(%rax)
 * mov    %rax,(%r14)
 * mov    %rbx,%rdi
 * callq  *0x8(%rbx)
 * mov    %rbx,%rdi
 * callq  0x400880 <_ZdlPv@plt>
 * mov    %r14,%rax
 * add    $0x8,%rsp
 * pop    %rbx
 * pop    %r14
 * retq
 */

//<-
int main(int, char** argv) {
  std::system(("gdb -batch -ex 'file " + std::string{argv[0]} + "' -ex 'disassemble test'").c_str());
}
//->
