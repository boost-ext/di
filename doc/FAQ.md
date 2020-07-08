* ### Dependency Inversion vs Dependency Injection
In principle Dependency Inversion might be implemented using DI or Service Locator (SL).
The latter is more like a 'God' object. It has a lot of benefits but, probably, the same
amount of cons such as that SL requires that SL will be the only constructor parameter for any class which can be created using it.
DI, on the other hand, is less intrusive and easier to follow than SL.

>

* ### Why C++14 is required for [Boost].DI?
At first [Boost].DI was implemented using C++03 standard with a switch to C++11 when available/possible.
However, this approach, although was/is working just fine, was/is really slow to compile and hard to maintain too.
Moreover, interface for `modules` wasn't perfect due to missing return type deduction for functions in C++11.
Therefore, I made a decision to reimplement the core using C++14 features which sped up compilation times dramatically
and make the interface cleaner and easier to maintain. Nevertheless, effort was made to support the newest versions of
popular C++ compilers including GCC/Clang and MSVC. If you are interested in C++03 version of the library, please, take
a look into [cpp03](https://github.com/boost-ext/di/tree/cpp03) branch. Please, also notice that this
branch is not maintained anymore.

>

* ### How the constructor deduction works without reflection support in C++?
Please check out [injection](overview.md#nutshell) design and http://boost-ext.github.io/di/cppnow-2016/#/7/11.

>

* ### Can I inject templates/concepts?
Yes, concepts/templates can be injected.
:w

```cpp
template <class T = class Greater>
struct example { 
  using type = T;
};

struct hello {};

int main() {
  const auto injector = di::make_injector(
    di::bind<class Greater>().to<hello>()
  );

  auto object = injector.create<example>();
  static_assert(std::is_same<hello, decltype(object)::type>{});
}
```

>

* ### Can I use XML in order to set-up bindings configuration?
Yes, however, XML injection is not part of the [Boost].DI release.
Please, check out [XML Extension](extensions.md#xml-injection).

>

* ### Since [Boost].DI has a compile time approach can I have dynamic bindings?
Yes, [Boost].DI supports dynamic bindings.

```cpp
auto use_gui_view = true/false;

auto injector = di::make_injector(
  di::bind<iview>.to([&](const auto& injector) -> iview& {
    if (use_gui_view)
      return injector.template create<gui_view&>();
    else
      return injector.template create<text_view&>();
  })
, di::bind<>.to(42) // renderer device
);
```

Check out full example of [Dynamic Bindings](examples.md#dynamic-bindings).
