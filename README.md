<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-experimental/di/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-experimental%2Fdi.svg)</a>
<a href="https://travis-ci.org/boost-experimental/di" target="_blank">![Build Status](https://img.shields.io/travis/boost-experimental/di/cpp14.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/boost-experimental/di" target="_blank">![Build Status](https://img.shields.io/appveyor/ci/boost-experimental/di/cpp14.svg?label=windows)</a>
<a href="https://coveralls.io/r/boost-experimental/di?branch=cpp14" target="_blank">![Coveralls](http://img.shields.io/coveralls/boost-experimental/di/cpp14.svg)</a>
<a href="http://github.com/boost-experimental/di/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/boost-experimental/di.svg)</a>

Boost.DI: C++ Dependency Injection
===============================================

> **"Let's make some coffee!"**
<p align="center"><img src="https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/coffee_maker.png" alt="coffee maker"/></p>
```cpp
No Dependency injection                 | Dependency Injection
----------------------------------------|--------------------------------------------
class coffee_maker {                    | class coffee_maker {
public:                                 | public:
    void brew() {                       |   coffee_maker(shared_ptr<iheater> heater
        heater->on();                   |              , unique_ptr<ipump> pump)
        pump->pump();                   |         : heater(heater), pump(move(pump))
        clog << "coffee"! << endl;      |     { }
        heater->off();                  |
    }                                   |     void brew() {
                                        |         heater->on();
private:                                |         pump->pump();
    shared_ptr<iheater> heater =        |         clog << "coffee!" << endl;
        make_shared<electric_heater>(); |         heater->off();
                                        |     }
    unique_ptr<ipump> pump =            |
        make_unique<heat_pump>(heater); | private:
};                                      |     shared_ptr<iheater> heater;
                                        |     unique_ptr<ipump> pump;
                                        | };
                                        |
```

---------------------------------------

[](GENERATE_TOC_BEGIN)

* [Introduction](http://boost-experimental.github.io/di/index.html)
    * [Why Dependency Injection?](http://boost-experimental.github.io/di/index.html#why-dependency-injection)
    * [Why Boost.DI?](http://boost-experimental.github.io/di/index.html#why-boostdi)
    * [Acknowledgements](http://boost-experimental.github.io/di/index.html#acknowledgements)
* [Overview](http://boost-experimental.github.io/di/overview/index.html)
    * [Quick Start](http://boost-experimental.github.io/di/overview/index.html#quick-start)
    * [Dependencies](http://boost-experimental.github.io/di/overview/index.html#dependencies)
    * [Supported/tested compilers](http://boost-experimental.github.io/di/overview/index.html#supportedtested-compilers)
    * [Configuration](http://boost-experimental.github.io/di/overview/index.html#configuration)
    * [Other Libraries](http://boost-experimental.github.io/di/overview/index.html#other-libraries)
    * [Performance](http://boost-experimental.github.io/di/overview/index.html#performance)
    * [Error messages](http://boost-experimental.github.io/di/overview/index.html#error-messages)
* [Tutorial](http://boost-experimental.github.io/di/tutorial/index.html)
    * [1. Create events and states](http://boost-experimental.github.io/di/tutorial/index.html#1-create-events-and-states)
* [Cheatsheet](http://boost-experimental.github.io/di/cheatsheet/index.html)
* [Quick User Guide](http://boost-experimental.github.io/di/quick_user_guide/index.html)
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
    * [Config](http://boost-experimental.github.io/di/user_guide/index.html#config)
* [Examples](http://boost-experimental.github.io/di/examples/index.html)
    * [Hello World](http://boost-experimental.github.io/di/examples/index.html#hello-world)
    * [Bindings](http://boost-experimental.github.io/di/examples/index.html#bindings)
    * [Dynamic Bindings](http://boost-experimental.github.io/di/examples/index.html#dynamic-bindings)
    * [Forward Bindings](http://boost-experimental.github.io/di/examples/index.html#forward-bindings)
    * [Multiple Bindings](http://boost-experimental.github.io/di/examples/index.html#multiple-bindings)
    * [Binding Non-owning Pointer](http://boost-experimental.github.io/di/examples/index.html#binding-non-owning-pointer)
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
* [Extensions](http://boost-experimental.github.io/di/extensions/index.html)
    * [Constructor Bindings](http://boost-experimental.github.io/di/extensions/index.html#constructor-bindings)
    * [Assisted Injection](http://boost-experimental.github.io/di/extensions/index.html#assisted-injection)
    * [Concepts](http://boost-experimental.github.io/di/extensions/index.html#concepts)
    * [Lazy](http://boost-experimental.github.io/di/extensions/index.html#lazy)
    * [Named Parameters](http://boost-experimental.github.io/di/extensions/index.html#named-parameters)
    * [XML Injection](http://boost-experimental.github.io/di/extensions/index.html#xml-injection)
    * [Types Dumper](http://boost-experimental.github.io/di/extensions/index.html#types-dumper)
    * [UML Dumper](http://boost-experimental.github.io/di/extensions/index.html#uml-dumper)
    * [Mocks Provider](http://boost-experimental.github.io/di/extensions/index.html#mocks-provider)
    * [Scoped Scope](http://boost-experimental.github.io/di/extensions/index.html#scoped-scope)
    * [Session Scope](http://boost-experimental.github.io/di/extensions/index.html#session-scope)
    * [Shared Scope](http://boost-experimental.github.io/di/extensions/index.html#shared-scope)
* [Projects](http://boost-experimental.github.io/di/projects/index.html)
    * [Experimental Boost.MSM-lite](http://boost-experimental.github.io/di/projects/index.html#experimental-boostmsm-lite)
    * [Automatic Mocks Injector](http://boost-experimental.github.io/di/projects/index.html#automatic-mocks-injector)
* [CHANGELOG](http://boost-experimental.github.io/di/CHANGELOG/index.html)
    * [ [1.0.0] - TBD](http://boost-experimental.github.io/di/CHANGELOG/index.html#-100-tbd)
    * [ [0.5.0] - 2015-01-12](http://boost-experimental.github.io/di/CHANGELOG/index.html#-050-2015-01-12)
    * [ [0.1.0] - 2014-08-15](http://boost-experimental.github.io/di/CHANGELOG/index.html#-010-2014-08-15)
* [TODO](http://boost-experimental.github.io/di/TODO/index.html)

[](GENERATE_TOC_END)

---

**Disclaimer** `Boost.DI` is not an official Boost library.
