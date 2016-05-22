//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
template <class T, class... TArgs>
struct ifactory {
  virtual ~ifactory() noexcept = default;
  virtual std::unique_ptr<T> create(TArgs&&...) const = 0;
};

template <class, class, class>
struct factory_impl;

template <class TInjector, class T, class I, class... TArgs>
struct factory_impl<TInjector, T, ifactory<I, TArgs...>> : ifactory<I, TArgs...> {
  factory_impl(const TInjector& injector) : injector_((TInjector&)injector) {}

  std::unique_ptr<I> create(TArgs&&... args) const override {
    // clang-format off
    auto injector = di::make_injector(
      std::move(injector_)
    , di::bind<TArgs>().to(std::forward<TArgs>(args))[di::override]...
    );
    // clang-format on

    return injector.template create<std::unique_ptr<T>>();
  }

 private:
  TInjector& injector_;
};

template <class T>
struct factory {
  template <class TInjector, class TDependency>
  auto operator()(const TInjector& injector, const TDependency&) const noexcept {
    static auto sp = std::make_shared<factory_impl<TInjector, T, typename TDependency::expected>>(injector);
    return sp;
  }
};

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy1() = 0;
};
//->

struct implementation : interface {
  void dummy1() override {}
};

struct implementation_with_args : interface {
  implementation_with_args(int i, double d) {
    assert(42 == i);
    assert(87.0 == d);
  }
  void dummy1() override {}
};

struct implementation_with_injected_args : interface {
  implementation_with_injected_args(int i, double d) {
    assert(123 == i);
    assert(87.0 == d);
  }
  void dummy1() override {}
};

/*<<example `example` class>>*/
class example {
 public:
  example(const ifactory<interface>& f1, const ifactory<interface, int, double>& f2, const ifactory<interface, double>& f3) {
    assert(dynamic_cast<implementation*>(f1.create().get()));
    assert(dynamic_cast<implementation_with_args*>(f2.create(42, 87.0).get()));
    assert(dynamic_cast<implementation_with_injected_args*>(f3.create(87.0).get()));
  }
};

int main() {
  //<<define injector>>
  auto module = [] { return di::make_injector(di::bind<>().to(123)); };

  // clang-format off
  auto injector = di::make_injector(module()

  //<<bind factory interface to implementation>>
  , di::bind<ifactory<interface>>().to(factory<implementation>{})

  //<<bind factory interface with parameters to implementation_with_args>>
  , di::bind<ifactory<interface, int, double>>().to(factory<implementation_with_args>{})

  //<<bind factory interface with some parameters injected to implementation_with_injected_args>>
  , di::bind<ifactory<interface, double>>().to(factory<implementation_with_injected_args>{})
  );
  // clang-format on

  /*<<create `example`>>*/
  injector.create<example>();
}
