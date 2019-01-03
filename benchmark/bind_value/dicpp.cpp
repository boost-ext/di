//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <di/registry.hpp>
#include <di/injector.hpp>

void module(di::registry& r) {
  // clang-format off
  r.add(r.type<int>().instance(new int{42}));
  // clang-format on
}

auto test() {
  di::injector injector;
  injector.install(module);
  return injector.construct<int>();
}

//<-
int main() {}
/**
 * ASM x86-64
 *
 * push   %rbx
 * sub    $0xb0,%rsp
 * lea    0x20(%rsp),%rbx
 * xor    %esi,%esi
 * mov    %rbx,%rdi
 * callq  0x40af50 <_ZN2di8injectorC2ENS0_8localityE>
 * movq   $0x404b10,(%rsp)
 * movq   $0x40ab00,0x18(%rsp)
 * movq   $0x40ab10,0x10(%rsp)
 * mov    %rbx,%rdi
 * callq  0x404b10 <_Z6moduleRN2di8registryE>
 * mov    0x10(%rsp),%rax
 * test   %rax,%rax
 * je     0x404d61 <_Z4testv+81>
 * lea    (%rsp),%rdi
 * mov    $0x3,%edx
 * mov    %rdi,%rsi
 * callq  *%rax
 * lea    0x20(%rsp),%rdi
 * callq  0x40ab40 <_ZNK2di8registry9constructIiEENS_6traits13returned_typeIT_E4typeEv>
 * mov    %eax,%ebx
 * lea    0x20(%rsp),%rdi
 * callq  0x40b390 <_ZN2di8registryD2Ev>
 * mov    %ebx,%eax
 * add    $0xb0,%rsp
 * pop    %rbx
 * retq
 * mov    %rax,%rbx
 * mov    0x10(%rsp),%rcx
 * test   %rcx,%rcx
 * je     0x404daa <_Z4testv+154>
 * lea    (%rsp),%rdi
 * mov    $0x3,%edx
 * mov    %rdi,%rsi
 * callq  *%rcx
 * jmp    0x404daa <_Z4testv+154>
 * mov    %rax,%rdi
 * callq  0x405f60 <__clang_call_terminate>
 * mov    %rax,%rbx
 * lea    0x20(%rsp),%rdi
 * callq  0x40b390 <_ZN2di8registryD2Ev>
 * mov    %rbx,%rdi
 * callq  0x404840 <_Unwind_Resume@plt>
 * mov    %rax,%rdi
 * callq  0x405f60 <__clang_call_terminate>
 */
//->
