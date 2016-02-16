// clang-format off
                   +---------------------------------------+
                   |  Boost.DI                             |
                   |  $CXX -std=c++14 -fno-exceptions -O2  |
                   |  #compiles in 0.2s!                   |
                   |                                       |
                   +---------------------------------------+                                                            +--------------------------------------+
                                                                                                   +------------------->+One header (3k lines, no dependencies)|
                                                                                                   |                    +--------------------------------------+
                                                                          #include <boost/di.hpp>  +
                                                                                                                                                          +------------------------------+
+------------------------------------+---------------------------------+  namespace di = boost::di;                                                       |Same as make_unique_ptr       |
| Can inject                         |                                 |                                                                                  |                              |
| using uniform initialization T{...}|   +--------------------+        +  struct uniform {                                                                +------------------------------+
| using direct initialization T(...) |   | conversion between |             bool &b;                                                                      +------------------------------+
|                                    |   | std::shared_ptr and+-----------+ boost::shared_ptr<interface> sp;    +-----------------------------------------+ push   %rax                  |
| Deduce constructor parameters      |   | boost::shared_ptr  |           };                                                                              | mov    $0x8,%edi             |
| without reflection!                |   +--------------------+                                                                                           | callq  0x4007b0 <_Znwm@plt>  |
|                                    |                                   +class direct {                                                                  | mo^q   $0x400a30,(%rax)      |
|                                    |                                   |public:                                                                         | mov    $0x8,%esi             |
|                                    |                                   |  direct(const uniform &uniform                                                 | mov    %rax,%rdi             |
|                                    |                                   |       , std::shared_ptr<interface> sp)                                         | callq  0x400960 <_ZdlPvm>    |
+------------------------------------+------------------------^----------+      : uniform_(uniform)                                                       | mov    $0x1,%eax             |
                                                              |                 , sp_(sp) {}                                                              | pop    %rdx                  |
                                                              |                                                                                           | retq                         |
                                                              |             const uniform &uniform_;                                                      +---------------------------^--+
                                                              |             std::shared_ptr<interface> sp_;                                                                           |
                                                              |           };                                                                                                          |
                                                              |                                                                                                                       |
                           +-------------------------+        +----------+class hello_world {                                                                                         |
                           | Scope deduction based   |                    public:                                                                                                     |
                           | on parameter type       |                      hello_world(std::unique_ptr<direct> d                          +------------------------------------------+--------------------------+
                           | T& -> singleton         +<-----------------------------+ , interface &ref                                     |`return injector.createvintv()` generates the same asm as `return 42`|
                           | shared_ptr -> singleton |                                , int i)    +---------------------------------------->    $0x2a,%eax                            |                          |
                           | T -> unique             |                          : i_(i) {                                                  |r tq                                      |                          |
                           | T* -> unique            |                        assert(false == d.uniform_.b);                               +-----------------------------------------------------------------^---+
                           | unique_ptr -> unique    |                        assert(d.sp_ == d.uniform_.sp); +                                                                       |                      |
                           +-------------------------+                        assert(&ref == &d.sp.get());    |                                                                       |                      |
                                                                            }                                 |                                                                       |                      |
                                                                                                              |                                                                       |                      |
                                                                            auto run() const {                |                 +-------------------------+                           |                      |
                                                                              return i_ == 42;                +---------------> |  same shared_ptr and ref|                           |                      |
                                                                            }                                                   +-------------------------+                           |                      |
                                                                                                                                                                                      |                      |
                                                                          private:                                                                                                    |                      |
                                                                            int i_ = 0;                                                                                               |                      |
                                                                          };                                                                                                          |                      |
                                                                                                                                                                                      |                      |
                                                                          int main() {                                                                                                |                      |
                                                                            auto runtime_value = false;                                                                               |                      |
                                                                                                                                                                                      |                      |
                                                                 +--------+ auto module = [&] {                                                                                       |                      |
                                                                 |            return di::make_injector(                                                                               |                      |
                                           +-----------------+   |              di::bind<>().to(runtime_value)                                                                        |                      |
                                           | module split    |   |            );        |                                                                                             |                      |
                                           | configuration   <---+          };          +--------------------------------------->                                                     |                      |
                                           +-----------------+   |                                                                                                                    |                      |
                                                                 |          auto injector = di::make_injector(                                                                        |                      |
                                                                 |            di::bind^interface>().to^implementation^() +------------------------------------------------------------+                      |
                                                                 |          , di::bind<>().to(42)  +---------------------------------------------------------------------------------------------------------+
                                                                 ^--------+ , module()
                                                                            );

                                                                            auto object = injector.create<hello_world>();
                                                                            assert(object.run());               +
                                                                          }                                     |
                                                                                                                |            +-------------------------------------------------------------+
                                                                                                                |            | Compile time creation guarantee!                            |
                                                                                                                +-----------^+ Short/Compile time error messages whentype can't be created |
                                                                                                                             +-------------------------------------------------------------+
