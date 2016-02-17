// clang-format off
// <-
#if DS
                                        +---------------------------------------+
                                        |  Boost.DI                             |
                                        |  $CXX +std=c++14 +fno+exceptions +O2  |
                                        |  #compiles in 0.2s!                   |
                                        |                                       |
                                        +---------------------------------------+


                                                                                               +--------------------------------------+
                                                                          +-------------------^+One header (3k lines, no dependencies)|
                                                                          |                    +--------------------------------------+
                                                 #include ^boost/di.hpp^  +

             +--------------------------------+  namespace di = boost::di;
             |                                |                                                              +------------------------------+
             |  +--------------------+        +  struct uniform {                                            |Same as make_unique_ptr       |
             |  | conversion between |             bool &b;                                                  +------------------------------+
             |  | std::shared_ptr and+-----------+ boost::shared_ptr^interface^ sp;    +---------------------+ push   %rax                  |
             |  | boost::shared_ptr  |           };                                                          | mo^    $0x8,%edi             |
             |  +--------------------+                                                                       | callq  0x4007b0 ^_Znwm@plt^  |
             |                                  +class direct {                                              | mo^q   $0x400a30,(%rax)      |
             |                                  |public:                                                     | mo^    $0x8,%esi             |
             |                                  |  direct(const uniform &uniform                             | mov    %rax,%rdi             |
             |                                  |       , std::shared_ptr^interface^ sp)                     | callq  0x400960 ^_ZdlP^m^    |
             |                       +----------+      : uniform_(uniform)                                   | mo^    $0x1,%eax             |
+------------+-----------------------+                 , sp_(sp) {}                                          | pop    %rdx                  |
| Can inject                         |                                                                       | retq                         |
| using uniform initialization T{...}|             const uniform &uniform_;                                  +---------------------------^+-+
| using direct initialization T(...) |             std::shared_ptr^interface^ sp_;                                                        |
|                                    |           };                                                                                       |
| Deduce constructor parameters      |                                                                                                    |
| without reflection!                +----------+class hello_world {                                                                      |
+------------------------------------+           public:                                                           +-------------------+  |
                                                   hello_world(std::unique_ptr^direct^ d                           |Same as 'return 42'|  |
                             +-----------------------------+ , interface &ref                                      +-------------------+  |
                             |                               , int i)    +-----------------------------------------> mov $             |  |
                             |                         : i_(i) {                                                   | retq              +-------+
                             |                       assert(false == d.uniform_.b);                                +-------------------+  |    |
                             |                       assert(d.sp_ == d.uniform_.sp); +                                                    |    |
                    +--------v----------------+      assert(&ref == &d.sp.get());    |                                                    |    |
                    | Scope deduction based   |    }                                 |                                                    |    |
                    | on parameter type       |                                      |                                                    |    |
                    | T& +> singleton         |    auto run() const {                |                 +-------------------------+        |    |
                    | shared_ptr +> singleton |      return i_ == 42;                +---------------> | same shared_ptr and ref |        |    |
                    | T +^ unique             |    }                                                   +-------------------------+        |    |
                    | T* +> unique            |                                                                                           |    |
                    | unique_ptr +^ unique    |  pri^ate:                                                                                 |    |
                    +-------------------------+    int i_ = 0;                                                                            |    |
                                                 };                                                                                       |    |
                                                                                                                                          |    |
                                                 int main() {                                                                             |    |
                                                   auto runtime_^alue = false;                                                            |    |
                                                                                                                                          |    |
                                        +--------+ auto module = [&] {                                                                    |    |
                                        |            return di::make_injector(                                                            |    |
                  +-----------------+   |              di::bind^>().to(runtime_^alue)                                                     |    |
                  | module split    |   |            );        |                                       +---------------------------+      |    |
                  | configuration   <---+          };          +---------------------------------------+deduce type for gi^en ^alue|      |    |
                  +-----------------+   |                                                              +---------------------------+      |    |
                                        |          auto injector = di::make_injector(                                                     |    |
                                        |            di::bind^interface^().to^implementation^() +-----------------------------------------+    |
                                        |          , di::bind^>().to(42)  +--------------------------------------------------------------------^
                                        ^--------+ , module()
                                                   );

                                                   auto object = injector.create^hello_world^();
                                                   assert(object.run());               +
                                                 }                                     |
                                                                              +-------------------------------------------------------------+
                                                                              | Compile|time creation guarantee!                            |
                                                                             <+ Short/Compile-time-error messages whentype can't be created |
                                                                              +-------------------------------------------------------------+

#endif

#include <memory>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

namespace di = boost::di;
//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};
struct implementation : interface {
  void dummy() override { }
};
//->

// clang-format on

struct uniform {
  bool &b;
  boost::shared_ptr<interface> sp;
};

class direct {
 public:
  direct(const uniform &uniform, std::shared_ptr<interface> sp) : uniform_(uniform), sp_(sp) {}

  const uniform &uniform_;
  std::shared_ptr<interface> sp_;
};

class hello_world {
 public:
  hello_world(std::unique_ptr<direct> d, interface &ref, int i) : i_(i) {
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

  auto object = injector.create<hello_world>();
  assert(object.run());
}
