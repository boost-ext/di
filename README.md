<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-experimental/di/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-experimental%2Fdi.svg)</a>
<a href="https://github.com/boost-experimental/di/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/boost-experimental/di.svg)</a>
<a href="http://boost-experimental.github.io/di/cpp14/boost/libs/di/doc/html/di/try_it_online.html" target="_blank">![Try Boost.DI Online](https://img.shields.io/badge/try%20boost.di-online-blue.svg)</a>
<a href="https://gitter.im/boost-experimental/di" target="_blank">![Gitter Chat](https://img.shields.io/badge/gitter-join%20chat%20%E2%86%92-brightgreen.svg)</a>

Boost.DI: C++ Dependency Injection
===============================================
[![Boost Libraries](https://raw.githubusercontent.com/boost-experimental/di/gh-pages/boost/boost.png)](http://www.boost.org)
<a href="https://travis-ci.org/boost-experimental/di" target="_blank">![Build Status](https://img.shields.io/travis/boost-experimental/di/cpp14.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/boost-experimental/di" target="_blank">![Build Status](https://img.shields.io/appveyor/ci/boost-experimental/di/cpp14.svg?label=windows)</a>
<a href="https://coveralls.io/r/boost-experimental/di?branch=cpp14" target="_blank">![Coveralls](http://img.shields.io/coveralls/boost-experimental/di/cpp14.svg)</a>
<a href="http://github.com/boost-experimental/di/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/boost-experimental/di.svg)</a>

> "Don't call us, we'll call you", Hollywood principle

**[Dependency Injection](http://www.youtube.com/watch?v=IKD2-MAkXyQ) (DI)** involves passing (injecting) one or more dependencies (or services) to a dependent object (or client) which become part of the client’s state.
It is like the Strategy Pattern, except the strategy is set once, at construction. DI enables loosely coupled designs, which are easier to maintain and test.

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

[](GENERATE_TOC_END)

---

**Disclaimer** `Boost.DI` is not an official Boost library.
