//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <memory>
//->
#include <fruit/fruit.h>

struct I {
  virtual ~I() noexcept = default;
  virtual void dummy() = 0;
};
struct Impl : I {
  INJECT(Impl()) {}
  void dummy() override {}
};

fruit::Component<I> module() {
  // clang-format off
  return fruit::createComponent()
    .bind<I, Impl>();
  // clang-format on
}

std::unique_ptr<I> test() {
  fruit::Injector<I> injector{module()};
  // get<unique_ptr> is not supported
  std::unique_ptr<I> object{injector.get<I*>()};
  return object;
}

//<-
int main() {}
/**
 * ASM x86-64
 *
 *  push   %r14
 *  push   %rbx
 *  sub    $0xc8,%rsp
 *  mov    %rdi,%r14
 *  lea    (%rsp),%rbx
 *  mov    %rbx,%rdi
 *  callq  0x401740 <_Z6modulev>
 *  lea    0xc0(%rsp),%rdi
 *  mov    %rbx,%rsi
 *  callq  0x401d30 <_ZN5fruit8InjectorIJ1IEEC2ENS_9ComponentIJS1_EEE>
 *  mov    0xa8(%rsp),%rdi
 *  test   %rdi,%rdi
 *  je     0x40186b <_Z4testv+59>
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  mov    0x90(%rsp),%rdi
 *  test   %rdi,%rdi
 *  je     0x40187d <_Z4testv+77>
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  mov    0x48(%rsp),%rdi
 *  test   %rdi,%rdi
 *  je     0x40188c <_Z4testv+92>
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  mov    0xc0(%rsp),%rbx
 *  lea    0x28(%rbx),%rdi
 *  mov    $0x40c360,%esi
 *  callq  0x40b990 <_ZNK5fruit4impl13SemistaticMapINS0_6TypeIdENS0_29SemistaticGraphInternalNodeIdEE2atES2_>
 *  mov    (%rax),%rsi
 *  test   $0xf,%sil
 *  jne    0x401934 <_Z4testv+260>
 *  mov    %rsi,%rax
 *  and    $0xfffffffffffffff0,%rax
 *  cmp    %rsi,%rax
 *  jne    0x40194d <_Z4testv+285>
 *  add    0x68(%rbx),%rsi
 *  mov    %rbx,%rdi
 *  callq  0x402b30
 * <_ZN5fruit4impl15InjectorStorage6getPtrI1IEEPT_NS0_15SemistaticGraphINS0_6TypeIdENS0_21NormalizedBindingDataEE13node_iteratorE>
 *  mov    %rax,(%r14)
 *  mov    %rbx,%rdi
 *  callq  0x401df0 <_ZN5fruit4impl15InjectorStorageD2Ev>
 *  mov    %rbx,%rdi
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  mov    %r14,%rax
 *  add    $0xc8,%rsp
 *  pop    %rbx
 *  pop    %r14
 *  retq
 *  mov    %rax,%r14
 *  mov    0xa8(%rsp),%rdi
 *  test   %rdi,%rdi
 *  je     0x401901 <_Z4testv+209>
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  mov    0x90(%rsp),%rdi
 *  test   %rdi,%rdi
 *  je     0x401913 <_Z4testv+227>
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  mov    0x48(%rsp),%rdi
 *  test   %rdi,%rdi
 *  je     0x401985 <_Z4testv+341>
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  mov    %r14,%rdi
 *  callq  0x4015a0 <_Unwind_Resume@plt>
 *  mov    %rax,%r14
 *  test   %rbx,%rbx
 *  je     0x401979 <_Z4testv+329>
 *  jmp    0x401969 <_Z4testv+313>
 *  mov    $0x40bc18,%edi
 *  mov    $0x40bc42,%esi
 *  mov    $0xb1,%edx
 *  mov    $0x40bca0,%ecx
 *  callq  0x4013f0 <__assert_fail@plt>
 *  mov    $0x40bdb3,%edi
 *  mov    $0x40bc42,%esi
 *  mov    $0xb4,%edx
 *  mov    $0x40bca0,%ecx
 *  callq  0x4013f0 <__assert_fail@plt>
 *  mov    %rax,%r14
 *  mov    %rbx,%rdi
 *  callq  0x401df0 <_ZN5fruit4impl15InjectorStorageD2Ev>
 *  mov    %rbx,%rdi
 *  callq  0x4013d0 <_ZdlPv@plt>
 *  movq   $0x0,0xc0(%rsp)
 *  mov    %r14,%rdi
 *  callq  0x4015a0 <_Unwind_Resume@plt>
 */
//->
