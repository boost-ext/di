<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-experimental/di/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-experimental%2Fdi.svg)</a>
<a href="https://travis-ci.org/boost-experimental/di" target="_blank">![Build Status](https://img.shields.io/travis/boost-experimental/di/cpp14.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/boost-experimental/di" target="_blank">![Build Status](https://img.shields.io/appveyor/ci/boost-experimental/di/cpp14.svg?label=windows)</a>
<a href="https://coveralls.io/r/boost-experimental/di?branch=cpp14" target="_blank">![Coveralls](http://img.shields.io/coveralls/boost-experimental/di/cpp14.svg)</a>
<a href="http://github.com/boost-experimental/di/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/boost-experimental/di.svg)</a>

---------------------------------------

Introduction
============

| **Experimental Boost.DI** | |
| - | - |
| Your C++14 header only Dependency Injection library with no dependencies ([__Try it online!__](http://boost-experimental.github.io/di/examples/index.html#hello-world)) | <a class="github-button" href="https://github.com/boost-experimental/di" data-style="mega" data-count-href="/boost-experimental/di/stargazers" data-count-api="/repos/boost-experimental/di#stargazers_count" data-count-aria-label="# stargazers on GitHub" aria-label="Star boost-experimental/di on GitHub">GitHub</a> |

<div class="important">
<h3 class="wy-text-neutral"><span class="fa fa-info-circle wy-text-info"></span>&nbsp; Latest Release: <b>v1.0.0</b> (February 28, 2016)</h3>
<a href="https://github.com/boost-experimental/di/releases" class="btn btn-success" style="margin-bottom:8px;" role="button"><span class="fa fa-download"></span>&nbsp; <b>Download</b></a> &nbsp;&nbsp;&nbsp;&nbsp; <a href="CHANGELOG/index.html" class="btn btn-info" style="margin-bottom:8px;" role="button"><span class="fa fa-reorder"></span>&nbsp; <b>Changelog</b></a> &nbsp;&nbsp;&nbsp;&nbsp; <a href="tutorial/index.html" class="btn btn-warning" style="margin-bottom:8px;" role="button"><span class="fa fa-gear"></span>&nbsp; <b>Tutorial</b></a> &nbsp;&nbsp;&nbsp;&nbsp; <a href="examples/index.html" class="btn btn-danger" style="margin-bottom:8px;" role="button"><span class="fa fa-book"></span>&nbsp; <b>Examples</b></a>
</div>

#What is Dependency Injection?
> "Don't call us, we'll call you", Hollywood principle

**[Dependency Injection](http://www.youtube.com/watch?v=IKD2-MAkXyQ) (DI)** involves passing (injecting) one or more dependencies (or services) to a dependent object (or client) which become part of the client’s state.
It is like the Strategy Pattern, except the strategy is set once, at construction. DI enables loosely coupled designs, which are easier to maintain and test.

> **"Let's make some coffee!"**
<p align="center"><img src="https://raw.githubusercontent.com/boost-experimental/di/cpp14/doc/images/coffee_maker.png" alt="coffee maker"/></p>
```cpp
                      No Dependency injection                 | Dependency Injection
                      ----------------------------------------|-------------------------------------------
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

###Do I need Dependency Injection?

###Real Life examples?

* [Match-3 Game](https://github.com/krzysztof-jusiak/match3)
    * Simple game in C++14 using SDL2 / Model View Controller / Meta State Machine / Dependency Injection

>

* [Automatic Mocks Injector](https://github.com/krzysztof-jusiak/mocks_injector)
    * Automatically create and inject required mocks to tested classes via constructors

>

* [Experimental Boost.MSM-lite](http://boost-experimental.github.io/msm-lite/)
    * C++14 header only eUML-like Meta State Machine library with no dependencies

###Why Dependency Injection?

* DI provides loosely coupled code (separation of business logic and object creation)
* DI provides easier to maintain code (different objects might be easily injected)
* DI provides easier to test code (fakes objects might be injected)

[![The Clean Code Talks - Don't Look For Things!](http://img.youtube.com/vi/RlfLCWKxHJ0/0.jpg)](http://www.youtube.com/watch?v=RlfLCWKxHJ0) | [![DAGGER 2 - A New Type of dependency injection](http://image.slidesharecdn.com/nr73mexetqiybd1qpcem-140529143342-phpapp01/95/dependency-injection-for-android-5-638.jpg?cb=1401392113)](http://www.youtube.com/watch?v=oK_XtfXPkqw) |
--- | --- |

###Why Boost.DI?

* Boost.DI has none or minimal run-time overhead - [Run-time performance](#run_time_performance)
* Boost.DI compiles fast - [Compile-time performance](#compile_time_performance)
* Boost.DI gives short diagnostic messages - [Error messages](#diagnostic_messages)
* Boost.DI is not intrusive
* Boost.DI reduces boilerplate code
* Boost.DI reduces testing effort
* Boost.DI gives better control of what and how is created
* Boost.DI gives better understanding about objects hierarchy
![CPP](https://raw.githubusercontent.com/boost-experimental/di/cpp14/example/hello_world.cpp)

###Boost.DI design goals

* Be as fast as possible (see [Performance](overview.md#performance))
* Compile as fast as possible (see [Performance](overview.md#performance))
* Give short and intuitive error messages (see [Error messages](overview.md#error_messages))
* Guarantee object creation at compile-time (see [Performance](overview.md#performance))
* Be as non-intrusive as possible (see [Performance](overview.md#performance))
* Be easy to extend (see [Extensions](extensions.md))

###Acknowledgements
* Thanks to Bartosz Kalinczuk for code review and tips how to improve `Boost.DI`
* Thanks to Sohail Somani for support and tips how to improve `Boost.DI`
* Thanks to Rob Stewart and Robert Ramey for documentation feedback and tips how to improve it
