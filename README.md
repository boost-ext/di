<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-ext/di/releases" target="_blank">![Version](https://img.shields.io/github/v/release/boost-ext/di)</a>
<a href="https://github.com/boost-ext/di/actions/workflows/build.yml" target="_blank">![Linux](https://github.com/boost-ext/di/actions/workflows/build.yml/badge.svg)</a>
<a href="https://codecov.io/gh/boost-ext/di" target="_blank">![Coveralls](https://codecov.io/gh/boost-ext/di/branch/cpp14/graph/badge.svg)</a>
<a href="https://godbolt.org/z/5qTKhf">![Try it online](https://img.shields.io/badge/try%20it-online-blue.svg)</a>

---------------------------------------

# boost-ext.di

Your C++14 **one header only** Dependency Injection library with no dependencies

<p align="center"><a href="https://www.youtube.com/watch?v=yVogS4NbL6U"><img src="doc/images/di_concept.png" alt="Dependency Injection"/></a></p>

> https://www.youtube.com/watch?v=yVogS4NbL6U

---

### Quick start

#### Download
> boost-ext.di requires only one file. Get the latest header [here!](https://raw.githubusercontent.com/boost-ext/di/cpp14/include/boost/di.hpp)

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
[Run this example on Wandbox](https://wandbox.org/permlink/Dpz6Isld1wxp3p5X).

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
  virtual ~interface() noexcept = default;
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
[Run this example on Wandbox](https://wandbox.org/permlink/4yBTlH1jvdrsRPes).

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
template<class ErrorPolicy = class TErrorPolicy>
class simple_updater {
public:
  void update() const {
    ErrorPolicy::on("update");
  }
};

template<class Updater = class TUpdater>
class example {
public:
  explicit example(const Updater& updater)
    : updater(updater)
  { }

  void update() {
    updater.update();
  }

private:
  const Updater& updater;
};

int main() {
  struct throw_policy {
    static void on(const std::string& str) {
      throw std::runtime_error(str);
    }
  };

  const auto injector = di::make_injector(
    di::bind<class TErrorPolicy>.to<throw_policy>(),
    di::bind<class TUpdater>.to<simple_updater>()
  );

  injector.create<example>().update();
  // Terminates with an uncaught exception because of our bound error policy
}
```
[Run this example on Wandbox](https://wandbox.org/permlink/mAzJQD8cTwboIxIx).

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
public:
  example(Exchange exchange, Engine engine)
    : exchange(std::move(exchange)), engine(std::move(engine))
  { }

private:
  Exchange exchange;
  Engine engine;
};

int main() {
  const auto injector = di::make_injector(
    di::bind<Streamable(class ExchangeStream)>.to<exchange>(),
    di::bind<Streamable(class EngineStream)>.to<engine>()
  );

  injector.create<example>();
}
```
[Run this example on Wandbox](https://wandbox.org/permlink/9QZTRsLkPNholmpj).

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

* [Introduction](https://boost-ext.github.io/di/index.html)
    * [Do I use a Dependency Injection already?](https://boost-ext.github.io/di/index.html#do-i-use-a-dependency-injection-already)
    * [Do I use Dependency Injection correctly?](https://boost-ext.github.io/di/index.html#do-i-use-dependency-injection-correctly)
    * [Do I need a Dependency Injection?](https://boost-ext.github.io/di/index.html#do-i-need-a-dependency-injection)
    * [STUPID vs SOLID - "Clean Code" Uncle Bob](https://boost-ext.github.io/di/index.html#stupid-vs-solid-clean-code-uncle-bob)
    * [Do I need a DI Framework/Library?](https://boost-ext.github.io/di/index.html#do-i-need-a-di-frameworklibrary)
    * [Manual DI - Wiring Mess (Avoid it by using [Boost].DI)](https://boost-ext.github.io/di/index.html#manual-di-wiring-mess-avoid-it-by-using-boostdi)
    * [Real Life examples?](https://boost-ext.github.io/di/index.html#real-life-examples)
    * [Why [Boost].DI?](https://boost-ext.github.io/di/index.html#why-boostdi)
    * [[Boost].DI design goals](https://boost-ext.github.io/di/index.html#boostdi-design-goals)
    * [Articles](https://boost-ext.github.io/di/index.html#articles)
    * [Videos](https://boost-ext.github.io/di/index.html#videos)
    * [[Boost::ext].DI](https://boost-ext.github.io/di/index.html#boostextdi)
    * [Dependency Injection In General](https://boost-ext.github.io/di/index.html#dependency-injection-in-general)
    * [Acknowledgements](https://boost-ext.github.io/di/index.html#acknowledgements)
* [Overview](https://boost-ext.github.io/di/overview.html)
    * [Quick Start](https://boost-ext.github.io/di/overview.html#quick-start)
    * [Dependencies](https://boost-ext.github.io/di/overview.html#dependencies)
    * [Supported/Tested compilers](https://boost-ext.github.io/di/overview.html#supportedtested-compilers)
    * [Configuration](https://boost-ext.github.io/di/overview.html#configuration)
    * [Exception Safety](https://boost-ext.github.io/di/overview.html#exception-safety)
    * [Thread Safety](https://boost-ext.github.io/di/overview.html#thread-safety)
    * [Error Messages](https://boost-ext.github.io/di/overview.html#error-messages)
    * [Performance](https://boost-ext.github.io/di/overview.html#performance)
* [Tutorial](https://boost-ext.github.io/di/tutorial.html)
    * [0. [Pre] Refactor towards DI](https://boost-ext.github.io/di/tutorial.html#0-pre-refactor-towards-di)
    * [1. [Basic] Create objects tree](https://boost-ext.github.io/di/tutorial.html#1-basic-create-objects-tree)
    * [2. [Basic] First steps with bindings](https://boost-ext.github.io/di/tutorial.html#2-basic-first-steps-with-bindings)
    * [3. [Basic] Decide the life times](https://boost-ext.github.io/di/tutorial.html#3-basic-decide-the-life-times)
    * [4. [Basic] Annotations to the rescue](https://boost-ext.github.io/di/tutorial.html#4-basic-annotations-to-the-rescue)
    * [5. [Basic] Split your configuration](https://boost-ext.github.io/di/tutorial.html#5-basic-split-your-configuration)
    * [6. [Advanced] Dump/Limit your types](https://boost-ext.github.io/di/tutorial.html#6-advanced-dumplimit-your-types)
    * [7. [Advanced] Customize it](https://boost-ext.github.io/di/tutorial.html#7-advanced-customize-it)
    * [8. [Advanced] Extend it](https://boost-ext.github.io/di/tutorial.html#8-advanced-extend-it)
* [Try It Online!](https://boost-ext.github.io/di/try_it.html)
* [Benchmarks](https://boost-ext.github.io/di/benchmarks.html)
    * [Performance](https://boost-ext.github.io/di/benchmarks.html#performance)
    * [C++ Libraries](https://boost-ext.github.io/di/benchmarks.html#c-libraries)
    * [C++ vs Java vs CLibraries](https://boost-ext.github.io/di/benchmarks.html#c-vs-java-vs-clibraries)
    * [Usage of C++ vs Java vs CLibraries](https://boost-ext.github.io/di/benchmarks.html#usage-of-c-vs-java-vs-clibraries)
* [User Guide](https://boost-ext.github.io/di/user_guide.html)
    * [Injector](https://boost-ext.github.io/di/user_guide.html#injector)
    * [Bindings](https://boost-ext.github.io/di/user_guide.html#bindings)
    * [Injections](https://boost-ext.github.io/di/user_guide.html#injections)
    * [Annotations](https://boost-ext.github.io/di/user_guide.html#annotations)
    * [Scopes](https://boost-ext.github.io/di/user_guide.html#scopes)
    * [Modules](https://boost-ext.github.io/di/user_guide.html#modules)
    * [Providers](https://boost-ext.github.io/di/user_guide.html#providers)
    * [Policies](https://boost-ext.github.io/di/user_guide.html#policies)
    * [Concepts](https://boost-ext.github.io/di/user_guide.html#concepts)
    * [Configuration](https://boost-ext.github.io/di/user_guide.html#configuration)
* [Examples](https://boost-ext.github.io/di/examples.html)
    * [Hello World](https://boost-ext.github.io/di/examples.html#hello-world)
    * [Bindings](https://boost-ext.github.io/di/examples.html#bindings)
    * [Dynamic Bindings](https://boost-ext.github.io/di/examples.html#dynamic-bindings)
    * [Forward Bindings](https://boost-ext.github.io/di/examples.html#forward-bindings)
    * [Is Creatable](https://boost-ext.github.io/di/examples.html#is-creatable)
    * [Multiple Bindings](https://boost-ext.github.io/di/examples.html#multiple-bindings)
    * [Binding Non-owning Pointer](https://boost-ext.github.io/di/examples.html#binding-non-owning-pointer)
    * [Binding Templates](https://boost-ext.github.io/di/examples.html#binding-templates)
    * [Binding To Constructor](https://boost-ext.github.io/di/examples.html#binding-to-constructor)
    * [Automatic Injection](https://boost-ext.github.io/di/examples.html#automatic-injection)
    * [Constructor Signature](https://boost-ext.github.io/di/examples.html#constructor-signature)
    * [Constructor Injection](https://boost-ext.github.io/di/examples.html#constructor-injection)
    * [Multiple Interface](https://boost-ext.github.io/di/examples.html#multiple-interface)
    * [Annotations](https://boost-ext.github.io/di/examples.html#annotations)
    * [Deduce Scope](https://boost-ext.github.io/di/examples.html#deduce-scope)
    * [Custom Scope](https://boost-ext.github.io/di/examples.html#custom-scope)
    * [Eager Singletons](https://boost-ext.github.io/di/examples.html#eager-singletons)
    * [Modules](https://boost-ext.github.io/di/examples.html#modules)
    * [Modules (hpp/cpp)](https://boost-ext.github.io/di/examples.html#modules-hppcpp)
    * [Custom Policy](https://boost-ext.github.io/di/examples.html#custom-policy)
    * [Custom Provider](https://boost-ext.github.io/di/examples.html#custom-provider)
    * [Pool Provider](https://boost-ext.github.io/di/examples.html#pool-provider)
    * [Configuration](https://boost-ext.github.io/di/examples.html#configuration)
    * [Polymorphism](https://boost-ext.github.io/di/examples.html#polymorphism)
    * [Inheritance](https://boost-ext.github.io/di/examples.html#inheritance)
    * [Type Erasure](https://boost-ext.github.io/di/examples.html#type-erasure)
    * [Function](https://boost-ext.github.io/di/examples.html#function)
    * [Variant](https://boost-ext.github.io/di/examples.html#variant)
    * [Templates](https://boost-ext.github.io/di/examples.html#templates)
    * [Concepts](https://boost-ext.github.io/di/examples.html#concepts)
* [Extensions](https://boost-ext.github.io/di/extensions.html)
    * [Injector](https://boost-ext.github.io/di/extensions.html#injector)
    * [Constructor Bindings](https://boost-ext.github.io/di/extensions.html#constructor-bindings)
    * [Contextual Bindings](https://boost-ext.github.io/di/extensions.html#contextual-bindings)
    * [Assisted Injection](https://boost-ext.github.io/di/extensions.html#assisted-injection)
    * [Extensible Injector](https://boost-ext.github.io/di/extensions.html#extensible-injector)
    * [Concepts](https://boost-ext.github.io/di/extensions.html#concepts)
    * [Factory](https://boost-ext.github.io/di/extensions.html#factory)
    * [Shared Factory](https://boost-ext.github.io/di/extensions.html#shared-factory)
    * [Lazy](https://boost-ext.github.io/di/extensions.html#lazy)
    * [Named Parameters](https://boost-ext.github.io/di/extensions.html#named-parameters)
    * [XML Injection](https://boost-ext.github.io/di/extensions.html#xml-injection)
    * [Serialize](https://boost-ext.github.io/di/extensions.html#serialize)
    * [Types Dumper](https://boost-ext.github.io/di/extensions.html#types-dumper)
    * [UML Dumper](https://boost-ext.github.io/di/extensions.html#uml-dumper)
    * [Heap Provider](https://boost-ext.github.io/di/extensions.html#heap-provider)
    * [Mocks Provider](https://boost-ext.github.io/di/extensions.html#mocks-provider)
    * [Runtime Provider](https://boost-ext.github.io/di/extensions.html#runtime-provider)
    * [Scoped Scope](https://boost-ext.github.io/di/extensions.html#scoped-scope)
    * [Session Scope](https://boost-ext.github.io/di/extensions.html#session-scope)
    * [Shared Scope](https://boost-ext.github.io/di/extensions.html#shared-scope)
* [FAQ](https://boost-ext.github.io/di/FAQ.html)
* [CHANGELOG](https://boost-ext.github.io/di/CHANGELOG.html)
    * [[1.3.0] - 2023-05-23](https://boost-ext.github.io/di/CHANGELOG.html#130-2023-05-23)
    * [[1.2.0] - 2020-07-21](https://boost-ext.github.io/di/CHANGELOG.html#120-2020-07-21)
    * [[1.1.0] - 2019-01-07](https://boost-ext.github.io/di/CHANGELOG.html#110-2019-01-07)
    * [[1.0.2] - 2018-01-04](https://boost-ext.github.io/di/CHANGELOG.html#102-2018-01-04)
    * [[1.0.1] - 2016-05-04](https://boost-ext.github.io/di/CHANGELOG.html#101-2016-05-04)
    * [[1.0.0] - 2016-02-22](https://boost-ext.github.io/di/CHANGELOG.html#100-2016-02-22)
    * [[0.5.0] - 2015-01-12](https://boost-ext.github.io/di/CHANGELOG.html#050-2015-01-12)
    * [[0.1.0] - 2014-08-15](https://boost-ext.github.io/di/CHANGELOG.html#010-2014-08-15)
[](GENERATE_TOC_END)

---

**Disclaimer** `boost-ext.di` is not an official Boost library.
