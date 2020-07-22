//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <fruit/fruit.h>

auto i = 42;  // bindInstance doesn't support l-value's

fruit::Component<int> module() {
  // clang-format off
  return fruit::createComponent().bindInstance(i);
  // clang-format on
}

auto test() {
  fruit::Injector<int> injector{module()};
  return injector.get<int>();
}

//<-
int main() {}
/**
 * ASM x86-64
 *
 * push   %rbp
 * push   %r14
 * push   %rbx
 * sub    $0xd0,%rsp
 * lea    0x8(%rsp),%rbx
 * mov    %rbx,%rdi
 * callq  0x400cd0 <_Z6modulev>
 * lea    0xc8(%rsp),%rdi
 * mov    %rbx,%rsi
 * callq  0x401170 <_ZN5fruit8InjectorIJiEEC2ENS_9ComponentIJiEEE>
 * mov    0xb0(%rsp),%rdi
 * test   %rdi,%rdi
 * je     0x400efa <_Z4testv+58>
 * callq  0x400b10 <_ZdlPv@plt>
 * mov    0x98(%rsp),%rdi
 * test   %rdi,%rdi
 * je     0x400f0c <_Z4testv+76>
 * callq  0x400b10 <_ZdlPv@plt>
 * mov    0x50(%rsp),%rdi
 * test   %rdi,%rdi
 * je     0x400f1b <_Z4testv+91>
 * callq  0x400b10 <_ZdlPv@plt>
 * mov    0xc8(%rsp),%rbx
 * mov    %rbx,%rdi
 * callq  0x4015e0
 * <_ZN5fruit4impl15InjectorStorage3getIiEENS0_4meta6DoEvalIFNS3_17RemoveAnnotationsENS3_4TypeIT_EEEE4type4typeEv>
 * mov    %eax,%ebp
 * test   %rbx,%rbx
 * je     0x400f42 <_Z4testv+130>
 * mov    %rbx,%rdi
 * callq  0x401230 <_ZN5fruit4impl15InjectorStorageD2Ev>
 * mov    %rbx,%rdi
 * callq  0x400b10 <_ZdlPv@plt>
 * mov    %ebp,%eax
 * add    $0xd0,%rsp
 * pop    %rbx
 * pop    %r14
 * pop    %rbp
 * retq
 * mov    %rax,%r14
 * mov    0xb0(%rsp),%rdi
 * test   %rdi,%rdi
 * je     0x400f65 <_Z4testv+165>
 * callq  0x400b10 <_ZdlPv@plt>
 * mov    0x98(%rsp),%rdi
 * test   %rdi,%rdi
 * je     0x400f77 <_Z4testv+183>
 * callq  0x400b10 <_ZdlPv@plt>
 * mov    0x50(%rsp),%rdi
 * test   %rdi,%rdi
 * je     0x400fb2 <_Z4testv+242>
 * callq  0x400b10 <_ZdlPv@plt>
 * mov    %r14,%rdi
 * callq  0x400bb0 <_Unwind_Resume@plt>
 * mov    %rax,%r14
 * test   %rbx,%rbx
 * je     0x400fa6 <_Z4testv+230>
 * mov    %rbx,%rdi
 * callq  0x401230 <_ZN5fruit4impl15InjectorStorageD2Ev>
 * mov    %rbx,%rdi
 * callq  0x400b10 <_ZdlPv@plt>
 * movq   $0x0,0xc8(%rsp)
 * mov    %r14,%rdi
 * callq  0x400bb0 <_Unwind_Resume@plt>
 */
//->
