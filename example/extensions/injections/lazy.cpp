//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[lazy
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy1() = 0;
};
struct type_expensive_to_create : interface {
  void dummy1() override {}
};

/*<<simple `lazy` implementation>>*/
template <class T>
class lazy {
  template <class TInjector>
  static auto create(const void *injector) {
    return ((TInjector *)injector)->template create<T>();
  }

 public:
  using boost_di_inject__ = di::inject<di::self<T>>;

  template <class TInjector>
  explicit lazy(const TInjector &i)
      : injector_((void *)&i), f(create<TInjector>) {}

  T get() const { return (*f)(injector_); }

 private:
  const void *injector_ = nullptr;
  T (*f)(const void *) = nullptr;
};

/*<<example `example` class>>*/
class example {
 public:
  explicit example(lazy<std::unique_ptr<interface>> l) : l(l) {}

  void initialize() {
    auto object = l.get();
    assert(dynamic_cast<type_expensive_to_create *>(object.get()));
  }

 private:
  lazy<std::unique_ptr<interface>> l;
};

int main() {
  /*<<define injector>>*/
  auto injector = di::make_injector(di::bind<interface>().to<type_expensive_to_create>());

  /*<<create `example`>>*/
  auto object = injector.create<example>();
  object.initialize();
}

//]
