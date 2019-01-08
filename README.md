<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-experimental/di/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-experimental%2Fdi.svg)</a>
<a href="https://travis-ci.org/boost-experimental/di" target="_blank">![Build Status](https://img.shields.io/travis/boost-experimental/di/cpp14.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/krzysztof-jusiak/di" target="_blank">![Build Status](https://img.shields.io/appveyor/ci/krzysztof-jusiak/di/cpp14.svg?label=windows)</a>
<a href="https://codecov.io/gh/boost-experimental/di" target="_blank">![Coveralls](https://codecov.io/gh/boost-experimental/di/branch/cpp14/graph/badge.svg)</a>
<a href="http://github.com/boost-experimental/di/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/boost-experimental/di.svg)</a>

---------------------------------------

# [Boost].DI

Your C++14 **one header only** Dependency Injection library with no dependencies ([__Try it online!__](http://boost-experimental.github.io/di/try_it/index.html))

### Quick start

#### Download
> [Boost].DI requires only one file. Get the latest header [here!](https://raw.githubusercontent.com/boost-experimental/di/cpp14/include/boost/di.hpp)

#### Include
```cpp
#include <boost/di.hpp>
namespace di = boost::di;
```

#### Compile

* **GCC/Clang**
  ```sh
  $CXX -std=c++14 -O2 -fno-exceptions -fno-rtti -Wall -Werror -pedantic-errors file.cpp
  ```
* **MSVC**
  ```sh
  cl /std:c++14 /Ox /W3 file.cpp
  ```

#### Quick guide - Create object graph

```cpp
class ctor {
public:
  explicit ctor(int i) : i(i) {}
  int i;
};

struct aggregate {
  double d;
};

class example {
 public:
  example(aggregate a, const ctor& c) {
    assert(87.0 == a.d);
    assert(42 == c.i);
  };
};

int main() {
  const auto injector = di::make_injector(
    di::bind<int>.to(42),
    di::bind<double>.to(87.0)
  );

  injector.create<example>();
}
```

<p align="center">
<table>
  <tr>
    <th></th>
    <th>Clang-3.8</th>
    <th>GCC-6</th>
    <th>MSVC-2015</th>
  </tr>

  <tr>
    <td>Compilation Time</td>
    <td>0.102s</td>
    <td>0.118s</td>
    <td>0.296s</td>
  </tr>

  <tr>
    <td>Binary size (stripped)</td>
    <td>6.2kb</td>
    <td>6.2kb</td>
    <td>105kb</td>
  </tr>

  <tr>
    <td>ASM x86-64</td>
    <td colspan="3">
      <pre><code>
xor eax, eax
retq
      </code></pre>
    </td>
  </tr>
</table>
</p>

#### Quick guide - Bind interfaces

```cpp
struct interface {
  virtual ~iworld() noexcept = default;
  virtual int get() const = 0;
};

class implementation : public interface {
public:
  int get() const override { return 42; }
};

struct example {
  example(std::shared_ptr<interface> i) {
    assert(42 == i->get());
  }
};

int main() {
  const auto injector = di::make_injector(
    di::bind<interface>.to<implementation>()
  );

  injector.create<std::unique_ptr<example>>();
}
```

<p align="center">
<table>
  <tr>
    <th></th>
    <th>Clang-3.8</th>
    <th>GCC-6</th>
    <th>MSVC-2015</th>
  </tr>

  <tr>
    <td>Compilation Time</td>
    <td>0.102s</td>
    <td>0.118s</td>
    <td>0.296s</td>
  </tr>

  <tr>
    <td>Binary size (stripped)</td>
    <td>6.2kb</td>
    <td>6.2kb</td>
    <td>105kb</td>
  </tr>

  <tr>
    <td>ASM x86-64 (same as `make_unique<example>`)</td>
    <td colspan="3">
      <pre><code>
push   %rbx
mov    %rdi,%rbx
mov    $0x8,%edi
callq  0x4008e0 <_Znwm@plt>
movq   $0x400c78,(%rax)
mov    %rax,(%rbx)
mov    %rbx,%rax
pop    %rbx
retq
      </code></pre>
    </td>
  </tr>
</table>
</p>

#### Quick guide - Bind templates

```cpp
template<class TPolicy = class TErrorPolicy>
class simple_updater : TPolicy {
  void update() {
    TPolicy::on("update");
  }
};

template<class T = class TUpdater>
class example {
public:
  explicit example(const TUpdater& updater)
    : updater(updater)
  { }

  void update() {
    updater.update();
  }

private:
  const TUpdater& updater;
};

int main() {
  struct throw_policy {
    void on(std::string_view str) {
      throw std::runtime_error(str);
    }
  };
  const auto injector = di::make_injector(
    di::bind<class TErrorPolicy>.to<throw_policy>(),
    di::bind<class TUpdater>.to<simple_updater>()
  );

  injector.create<example>().update();
}
```

<p align="center">
<table>
  <tr>
    <th></th>
    <th>Clang-3.8</th>
    <th>GCC-6</th>
    <th>MSVC-2015</th>
  </tr>

  <tr>
    <td>Compilation Time</td>
    <td>0.102s</td>
    <td>0.118s</td>
    <td>0.296s</td>
  </tr>

  <tr>
    <td>Binary size (stripped)</td>
    <td>6.2kb</td>
    <td>6.2kb</td>
    <td>105kb</td>
  </tr>

  <tr>
    <td>ASM x86-64</td>
    <td colspan="3">
      <pre><code>
xor eax, eax
retq
      </code></pre>
    </td>
  </tr>
</table>
</p>

#### Quick guide - Bind concepts

```cpp
struct Streamable {
 template<class T>
 auto requires(T&& t) -> decltype(
   int( t.read() ),
   t.write(int)
 );
};

template<class Exchange = Streamable(class ExchangeStream)
         class Engine   = Streamable(class EngineStream)>
class example {
  example(Exchange exchange, Engine engine)
    : exchange(exchange), engine(engine)
  { }
};

int main() {
  const auto injector = di::make_injector(
    di::bind<Streamable(class ExchangeStream)>.to<exchange>(),
    di::bind<Streamable(class EngineStream)>.to<engine>()
  );

  injector.create<example>();
}
```

<p align="center">
<table>
  <tr>
    <th></th>
    <th>Clang-3.8</th>
    <th>GCC-6</th>
    <th>MSVC-2015</th>
  </tr>

  <tr>
    <td>Compilation Time</td>
    <td>0.102s</td>
    <td>0.118s</td>
    <td>0.296s</td>
  </tr>

  <tr>
    <td>Binary size (stripped)</td>
    <td>6.2kb</td>
    <td>6.2kb</td>
    <td>105kb</td>
  </tr>

  <tr>
    <td>ASM x86-64</td>
    <td colspan="3">
      <pre><code>
xor eax, eax
retq
      </code></pre>
    </td>
  </tr>
</table>
</p>

---------------------------------------

### Documentation

[](GENERATE_TOC_BEGIN)

* [Introduction](http://boost-experimental.github.io/di/index.html)
    * [Do I use a Dependency Injection already?](http://boost-experimental.github.io/di/index.html#do-i-use-a-dependency-injection-already)
    * [Do I use Dependency Injection correctly?](http://boost-experimental.github.io/di/index.html#do-i-use-dependency-injection-correctly)
    * [Do I need a Dependency Injection?](http://boost-experimental.github.io/di/index.html#do-i-need-a-dependency-injection)
    * [STUPID vs SOLID - "Clean Code" Uncle Bob](http://boost-experimental.github.io/di/index.html#stupid-vs-solid-clean-code-uncle-bob)
    * [Do I need a DI Framework/Library?](http://boost-experimental.github.io/di/index.html#do-i-need-a-di-frameworklibrary)
    * [Manual DI - Wiring Mess (Avoid it by using [Boost].DI)](http://boost-experimental.github.io/di/index.html#manual-di-wiring-mess-avoid-it-by-using-boostdi)
    * [Real Life examples?](http://boost-experimental.github.io/di/index.html#real-life-examples)
    * [Why [Boost].DI?](http://boost-experimental.github.io/di/index.html#why-boostdi)
    * [[Boost].DI design goals](http://boost-experimental.github.io/di/index.html#boostdi-design-goals)
    * [Articles](http://boost-experimental.github.io/di/index.html#articles)
    * [Videos](http://boost-experimental.github.io/di/index.html#videos)
    * [Acknowledgements](http://boost-experimental.github.io/di/index.html#acknowledgements)
* [Overview](http://boost-experimental.github.io/di/overview/index.html)
    * [Quick Start](http://boost-experimental.github.io/di/overview/index.html#quick-start)
    * [Dependencies](http://boost-experimental.github.io/di/overview/index.html#dependencies)
    * [Supported/Tested compilers](http://boost-experimental.github.io/di/overview/index.html#supportedtested-compilers)
    * [Configuration](http://boost-experimental.github.io/di/overview/index.html#configuration)
    * [Exception Safety](http://boost-experimental.github.io/di/overview/index.html#exception-safety)
    * [Thread Safety](http://boost-experimental.github.io/di/overview/index.html#thread-safety)
    * [Error Messages](http://boost-experimental.github.io/di/overview/index.html#error-messages)
    * [Performance](http://boost-experimental.github.io/di/overview/index.html#performance)
* [Tutorial](http://boost-experimental.github.io/di/tutorial/index.html)
    * [0. [Pre] Refactor towards DI](http://boost-experimental.github.io/di/tutorial/index.html#0-pre-refactor-towards-di)
    * [1. [Basic] Create objects tree](http://boost-experimental.github.io/di/tutorial/index.html#1-basic-create-objects-tree)
    * [2. [Basic] First steps with bindings](http://boost-experimental.github.io/di/tutorial/index.html#2-basic-first-steps-with-bindings)
    * [3. [Basic] Decide the life times](http://boost-experimental.github.io/di/tutorial/index.html#3-basic-decide-the-life-times)
    * [4. [Basic] Annotations to the rescue](http://boost-experimental.github.io/di/tutorial/index.html#4-basic-annotations-to-the-rescue)
    * [5. [Basic] Split your configuration](http://boost-experimental.github.io/di/tutorial/index.html#5-basic-split-your-configuration)
    * [6. [Advanced] Dump/Limit your types](http://boost-experimental.github.io/di/tutorial/index.html#6-advanced-dumplimit-your-types)
    * [7. [Advanced] Customize it](http://boost-experimental.github.io/di/tutorial/index.html#7-advanced-customize-it)
    * [8. [Advanced] Extend it](http://boost-experimental.github.io/di/tutorial/index.html#8-advanced-extend-it)
* [Try It Online!](http://boost-experimental.github.io/di/try_it/index.html)
* [Benchmarks](http://boost-experimental.github.io/di/benchmarks/index.html)
    * [Performance](http://boost-experimental.github.io/di/benchmarks/index.html#performance)
    * [C++ Libraries](http://boost-experimental.github.io/di/benchmarks/index.html#c-libraries)
    * [C++ vs Java vs C Libraries](http://boost-experimental.github.io/di/benchmarks/index.html#c-vs-java-vs-c-libraries)
    * [Usage of C++ vs Java vs C Libraries](http://boost-experimental.github.io/di/benchmarks/index.html#usage-of-c-vs-java-vs-c-libraries)
* [User Guide](http://boost-experimental.github.io/di/user_guide/index.html)
    * [Injector](http://boost-experimental.github.io/di/user_guide/index.html#injector)
    * [Bindings](http://boost-experimental.github.io/di/user_guide/index.html#bindings)
    * [Injections](http://boost-experimental.github.io/di/user_guide/index.html#injections)
    * [Annotations](http://boost-experimental.github.io/di/user_guide/index.html#annotations)
    * [Scopes](http://boost-experimental.github.io/di/user_guide/index.html#scopes)
    * [Modules](http://boost-experimental.github.io/di/user_guide/index.html#modules)
    * [Providers](http://boost-experimental.github.io/di/user_guide/index.html#providers)
    * [Policies](http://boost-experimental.github.io/di/user_guide/index.html#policies)
    * [Concepts](http://boost-experimental.github.io/di/user_guide/index.html#concepts)
    * [Configuration](http://boost-experimental.github.io/di/user_guide/index.html#configuration)
* [Examples](http://boost-experimental.github.io/di/examples/index.html)
    * [Hello World](http://boost-experimental.github.io/di/examples/index.html#hello-world)
    * [Bindings](http://boost-experimental.github.io/di/examples/index.html#bindings)
    * [Dynamic Bindings](http://boost-experimental.github.io/di/examples/index.html#dynamic-bindings)
    * [Forward Bindings](http://boost-experimental.github.io/di/examples/index.html#forward-bindings)
    * [Is Creatable](http://boost-experimental.github.io/di/examples/index.html#is-creatable)
    * [Multiple Bindings](http://boost-experimental.github.io/di/examples/index.html#multiple-bindings)
    * [Binding Non-owning Pointer](http://boost-experimental.github.io/di/examples/index.html#binding-non-owning-pointer)
    * [Binding Templates](http://boost-experimental.github.io/di/examples/index.html#binding-templates)
    * [Binding To Constructor](http://boost-experimental.github.io/di/examples/index.html#binding-to-constructor)
    * [Automatic Injection](http://boost-experimental.github.io/di/examples/index.html#automatic-injection)
    * [Constructor Signature](http://boost-experimental.github.io/di/examples/index.html#constructor-signature)
    * [Constructor Injection](http://boost-experimental.github.io/di/examples/index.html#constructor-injection)
    * [Multiple Interface](http://boost-experimental.github.io/di/examples/index.html#multiple-interface)
    * [Annotations](http://boost-experimental.github.io/di/examples/index.html#annotations)
    * [Deduce Scope](http://boost-experimental.github.io/di/examples/index.html#deduce-scope)
    * [Custom Scope](http://boost-experimental.github.io/di/examples/index.html#custom-scope)
    * [Eager Singletons](http://boost-experimental.github.io/di/examples/index.html#eager-singletons)
    * [Modules](http://boost-experimental.github.io/di/examples/index.html#modules)
    * [Modules (hpp/cpp)](http://boost-experimental.github.io/di/examples/index.html#modules-hppcpp)
    * [Custom Policy](http://boost-experimental.github.io/di/examples/index.html#custom-policy)
    * [Custom Provider](http://boost-experimental.github.io/di/examples/index.html#custom-provider)
    * [Pool Provider](http://boost-experimental.github.io/di/examples/index.html#pool-provider)
    * [Configuration](http://boost-experimental.github.io/di/examples/index.html#configuration)
    * [Polymorphism](http://boost-experimental.github.io/di/examples/index.html#polymorphism)
    * [Inheritance](http://boost-experimental.github.io/di/examples/index.html#inheritance)
    * [Type Erasure](http://boost-experimental.github.io/di/examples/index.html#type-erasure)
    * [Variant](http://boost-experimental.github.io/di/examples/index.html#variant)
    * [Templates](http://boost-experimental.github.io/di/examples/index.html#templates)
    * [Concepts](http://boost-experimental.github.io/di/examples/index.html#concepts)
* [Extensions](http://boost-experimental.github.io/di/extensions/index.html)
    * [Injector](http://boost-experimental.github.io/di/extensions/index.html#injector)
    * [Constructor Bindings](http://boost-experimental.github.io/di/extensions/index.html#constructor-bindings)
    * [Contextual Bindings](http://boost-experimental.github.io/di/extensions/index.html#contextual-bindings)
    * [Assisted Injection](http://boost-experimental.github.io/di/extensions/index.html#assisted-injection)
    * [Extensible Injector](http://boost-experimental.github.io/di/extensions/index.html#extensible-injector)
    * [Concepts](http://boost-experimental.github.io/di/extensions/index.html#concepts)
    * [Factory](http://boost-experimental.github.io/di/extensions/index.html#factory)
    * [Shared Factory](http://boost-experimental.github.io/di/extensions/index.html#shared-factory)
    * [Lazy](http://boost-experimental.github.io/di/extensions/index.html#lazy)
    * [Named Parameters](http://boost-experimental.github.io/di/extensions/index.html#named-parameters)
    * [XML Injection](http://boost-experimental.github.io/di/extensions/index.html#xml-injection)
    * [Serialize](http://boost-experimental.github.io/di/extensions/index.html#serialize)
    * [Types Dumper](http://boost-experimental.github.io/di/extensions/index.html#types-dumper)
    * [UML Dumper](http://boost-experimental.github.io/di/extensions/index.html#uml-dumper)
    * [Heap Provider](http://boost-experimental.github.io/di/extensions/index.html#heap-provider)
    * [Mocks Provider](http://boost-experimental.github.io/di/extensions/index.html#mocks-provider)
    * [Runtime Provider](http://boost-experimental.github.io/di/extensions/index.html#runtime-provider)
    * [Scoped Scope](http://boost-experimental.github.io/di/extensions/index.html#scoped-scope)
    * [Session Scope](http://boost-experimental.github.io/di/extensions/index.html#session-scope)
    * [Shared Scope](http://boost-experimental.github.io/di/extensions/index.html#shared-scope)
* [FAQ](http://boost-experimental.github.io/di/FAQ/index.html)
* [CHANGELOG](http://boost-experimental.github.io/di/CHANGELOG/index.html)
    * [ [1.1.0] - 2019-01-07](http://boost-experimental.github.io/di/CHANGELOG/index.html#-110-2019-01-07)
    * [ [1.0.2] - 2018-01-04](http://boost-experimental.github.io/di/CHANGELOG/index.html#-102-2018-01-04)
    * [ [1.0.1] - 2016-05-04](http://boost-experimental.github.io/di/CHANGELOG/index.html#-101-2016-05-04)
    * [ [1.0.0] - 2016-02-22](http://boost-experimental.github.io/di/CHANGELOG/index.html#-100-2016-02-22)
    * [ [0.5.0] - 2015-01-12](http://boost-experimental.github.io/di/CHANGELOG/index.html#-050-2015-01-12)
    * [ [0.1.0] - 2014-08-15](http://boost-experimental.github.io/di/CHANGELOG/index.html#-010-2014-08-15)

[](GENERATE_TOC_END)

---

**Disclaimer** `Boost.DI` is not an official Boost library.
