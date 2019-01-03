//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
#include <type_traits>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
class interface {
 public:
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};

class implementation : public interface {
 public:
  implementation() { ++ctor_calls(); }
  void dummy() override {}

  static int& ctor_calls() {
    static auto calls = 0;
    return calls;
  }
};

template <class TDependency, class TInjector,
          std::enable_if_t<std::is_same<typename TDependency::scope, di::scopes::singleton>::value, int> = 0>
void create_singletons_eagerly_impl(const di::aux::type<TDependency>&, const TInjector& injector) {
  injector.template create<std::shared_ptr<typename TDependency::expected>>();
}

template <class TDependency, class TInjector,
          std::enable_if_t<!std::is_same<typename TDependency::scope, di::scopes::singleton>::value, int> = 0>
void create_singletons_eagerly_impl(const di::aux::type<TDependency>&, const TInjector&) {}

template <class... TDeps, class TInjector>
void create_singletons_eagerly(const di::aux::type_list<TDeps...>&, const TInjector& injector) {
  [](...) {}((create_singletons_eagerly_impl(di::aux::type<TDeps>{}, injector), 0)...);
}

template <class TInjector>
void eager_singletons(const TInjector& injector) {
  create_singletons_eagerly(typename TInjector::deps{}, injector);
}
//->

auto configuration = [] {
  // clang-format off
  return di::make_injector(
    di::bind<interface>().to<implementation>().in(di::singleton)
  , di::bind<int>().to(42)
  );
  // clang-format on
};

struct example {
  example(int i, std::shared_ptr<interface> object) {
    assert(42 == i);
    assert(dynamic_cast<implementation*>(object.get()));
  }
};

int main() {
  /*<<make injector configuration>>*/
  auto injector = di::make_injector(configuration());
  assert(0 == implementation::ctor_calls());

  /*<<eagerly initialize singletons>>*/
  eager_singletons(injector);
  assert(1 == implementation::ctor_calls());

  /*<<create `example` with already initialized singletons>>*/
  injector.create<example>();
  assert(1 == implementation::ctor_calls());
}
