//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// clang-format off
//<-
#if defined(BOOST_DI_EXAMPLE)
//->
+---------------------------------------------------+
|$CXX -std=c++14 -fno-exceptions -O2 example.cpp    |
|#Compiles in 0.4s!                                 |
+-----------------------------+---------------------+
                              |
                              \
                                  #include <boost/di.hpp> +-----------+
 +-----------------------------+                                      |
 |                             |  namespace di = boost::di;     +-----+--------------------------------+
 |                             +-+                              |One header (3k lines, no dependencies)|
 |  +-----------------------+    +struct uniform {              +--------------------------------------+
 |  |Automatic conversion   |       bool &b;
 |  |between std::shared_ptr+------+boost::shared_ptr<interface> sp;
 |  |and boost::shared_ptr  |     };
 |  +-----------------------+
 |                             +-+class direct {
 |                          +--+   public:                                 +---------------------------+
 |                          |       direct(const uniform &uniform          |ASM x86-64 == `make_unique`|
 |               +----------+            , std::shared_ptr<interface> sp)  +---------------------------+
 |               |                    : uniform_(uniform)                  |push   %rax                |
 |               |                  , sp_(sp)                              |mov    $0x8,%edi           |
 |               |                  {}                                     |callq  0x4007b0 <_Znwm@plt>|
 |               |                                                         |movq   $0x400a10,(%rax)    |
 | +-------------+----------+       const uniform &uniform_;               |mov    $0x8,%esi           |
 | |Inject  dependencies    |       std::shared_ptr<interface> sp_;        |mov    %rax,%rdi           |
 | |using T{...} or T(...)  |     };                                       |callq  0x400960 <_ZdlPvm>  |
 +-+without REFLECTION or   |                                +-------------+mov    $0x1,%eax           |
   |any changes/registration+-----+class example {           |             |pop    %rdx                |
   |in the code!            |      public:                   +             |retq                       |
   +------------------------+       example(std::unique_ptr<direct> d      +-------------------------+-+
                                   +--------+ , interface &ref                                       |
                                   |          , int i)+-------------------------------------------+  +-+
                                   |  : i_(i) {                                                   |    |
                                   |  assert(false == d->uniform_.b);                             |    |
                     +-------------+  assert(d->sp_.get() == d->uniform_.sp.get());               |    |
                     |                assert(&ref == d->sp_.get());     +                         |    |
    +----------------+---------+    }                         +         |                         |    |
    |Deduce scope based on     |                              |         |                         |    |
    |constructor parameter type|    auto run() const {        +---------+ +--------------------+  |    |
    |T -> unique               |      return i_ == 42;                  +-+The same shared_ptr,|  |    |
    |T& -> singleton           |    }                                     |reference provided  |  |    |
    |shared_ptr -> singleton   |                                          +--------------------+  |    |
    |unique_ptr |> unique      |   private:                                                       |    |
    +--------------------------+    int i_ = 0;                                                +--+    |
                                  };                                                           |       |
                                                                                               |       |
                                  int main() {                          +----------------------+--+    |
                                    auto runtime_value = false;         |ASM x86-64 == 'return 42'|    |
                                                                        +-------------------------+    |
                    +-------------+ auto module = [&] {                 |mov $0x2a,%eax           |    |
            +-------+-----------+     return di::make_injector(         |retq                     |    |
            |Split configuration|       di::bind<>().to(runtime_value)  +----+--------------------+    |
            |into modules       |     );                                     |                         |
            +-------+-----------+   };                                       |                         |
                    |         +----------------------------------------------+                         |
                    |         |     auto injector = di::make_injector(                                 |
                    |         |       di::bind<interface>().to<implementation>()+----------------------+
                    |         +---+ , di::bind<>().to(42)
                    +--------------+, module()                                     +---------------------+
                                    );                                  +----------+Compile time creation|
                                                                        +          |guarantee!           |
                                    auto object = injector.create<example>();      +---------------------+
                                    assert(object.run());  +
                                  }                        |
                                                           |  +----------------------------------------+
                                                           +--+Short compile time error messages!      |
                                                              |For example:                            |
                                                              |`abstract_type<interface>::is_not_bound`|
                                                              +----------------------------------------+
//<-
#endif

#include <memory>
#include <cassert>

#include <boost/di.hpp>

#if __has_include(<boost/shared_ptr.hpp>)
#include <boost/shared_ptr.hpp>
#endif

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation : interface {
  void dummy() override { }
};

// clang-format on

struct uniform {
  bool &b;
#if __has_include(<boost / shared_ptr.hpp>)
  boost::shared_ptr<interface> sp;
#else
  std::shared_ptr<interface> sp;
#endif
};

class direct {
 public:
  direct(const uniform &uniform, std::shared_ptr<interface> sp) : uniform_(uniform), sp_(sp) {}

  const uniform &uniform_;
  std::shared_ptr<interface> sp_;
};

class example {
 public:
  example(std::unique_ptr<direct> d, interface &ref, int i) : i_(i) {
    assert(false == d->uniform_.b);
    assert(d->sp_.get() == d->uniform_.sp.get());
    assert(&ref == d->sp_.get());
  }

  auto run() const { return i_ == 42; }

 private:
  int i_ = 0;
};

int main() {
  auto runtime_value = false;

  // clang-format off
  auto module = [&] {
    return di::make_injector(
      di::bind<>().to(runtime_value)
    );
  };

  auto injector = di::make_injector(
    di::bind<interface>().to<implementation>()
  , di::bind<>().to(42)
  , module()
  );
  // clang-format on

  /*<<create `example` - member function call>>*/
  {
    auto object = injector.create<example>();
    assert(object.run());
  }

  /*<<create `example` - free function call>>*/
  {
    auto object = di::create<example>(injector);
    assert(object.run());
  }
}
//->
