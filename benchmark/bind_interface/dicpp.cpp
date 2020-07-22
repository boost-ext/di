//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <memory>
//->
#include <di/registry.hpp>
#include <di/injector.hpp>
#include <di/constructor.hpp>

struct I {
  virtual ~I() noexcept = default;
  virtual void dummy() = 0;
};
struct Impl : I {
  DI_CONSTRUCTOR(Impl, ()) {}
  void dummy() override {}
};

void module(di::registry& r) {
  // clang-format off
  r.add(r.type<I>().implementation<Impl>());
  // clang-format on
}

std::unique_ptr<I> test() {
  di::injector injector;
  injector.install(module);
  std::unique_ptr<I> object{injector.construct<I*>()};
  return object;
}

//<-
int main() {}
/**
 * ASM x86-64
 *
 * push   %r14
 * push   %rbx
 * sub    $0xc8,%rsp
 * mov    %rdi,%r14
 * lea    0x20(%rsp),%rbx
 * xor    %esi,%esi
 * mov    %rbx,%rdi
 * callq  0x40c3d0 <_ZN2di8injectorC2ENS0_8localityE>
 * movq   $0x4044c0,(%rsp)
 * movq   $0x40bf80,0x18(%rsp)
 * movq   $0x40bf90,0x10(%rsp)
 * xorps  %xmm0,%xmm0
 * movaps %xmm0,0xb0(%rsp)
 * lea    0xb0(%rsp),%rsi
 * mov    %rbx,%rdi
 * callq  0x4047e0
 * <_ZN2di8registry3addI1IN5boost3mpl6m_itemINS_6detail4tags14implementationE4ImplNS4_3mapIN4mpl_2naESC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_SC_EEEEEEvRKNS6_20registration_builderIT_T0_EE>
 * mov    0x10(%rsp),%rax
 * test   %rax,%rax
 * je     0x404549 <_Z4testv+105>
 * lea    (%rsp),%rdi
 * mov    $0x3,%edx
 * mov    %rdi,%rsi
 * callq  *%rax
 * lea    0x20(%rsp),%rdi
 * callq  0x40bfc0 <_ZNK2di8registry9constructIP1IEENS_6traits13returned_typeIT_E4typeEv>
 * mov    %rax,(%r14)
 * lea    0x20(%rsp),%rdi
 * callq  0x40c7d0 <_ZN2di8registryD2Ev>
 * mov    %r14,%rax
 * add    $0xc8,%rsp
 * pop    %rbx
 * pop    %r14
 * retq
 * mov    %rax,%rbx
 * mov    0x10(%rsp),%rcx
 * test   %rcx,%rcx
 * je     0x404596 <_Z4testv+182>
 * lea    (%rsp),%rdi
 * mov    $0x3,%edx
 * mov    %rdi,%rsi
 * callq  *%rcx
 * jmp    0x404596 <_Z4testv+182>
 * mov    %rax,%rdi
 * callq  0x405890 <__clang_call_terminate>
 * mov    %rax,%rbx
 * lea    0x20(%rsp),%rdi
 * callq  0x40c7d0 <_ZN2di8registryD2Ev>
 * mov    %rbx,%rdi
 * callq  0x4041f0 <_Unwind_Resume@plt>
 * mov    %rax,%rdi
 * callq  0x405890 <__clang_call_terminate>
 */
//->
