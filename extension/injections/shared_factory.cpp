//
// Copyright (c) 2012-2017 Kanstantsin Chernik
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

//<<rebind only if expected and current type are same>>
template <class TExpected, class T>
struct injector_rebinder {
  template <class TInjector>
  auto rebind(TInjector& injector) {
    return std::move(injector);
  }
};

template <class T>
struct injector_rebinder<T, T> {
  template <class TInjector>
  auto rebind(TInjector& injector) {
    return di::make_injector(std::move(injector), di::bind<T>().in(di::unique)[di::override]);
  }
};

template <class T, class TFunc>
struct shared_factory_impl {
  shared_factory_impl(TFunc&& creation_func) : creation_func_(std::move(creation_func)), is_created_(false) {}

  shared_factory_impl(const shared_factory_impl& other)
      : creation_func_(std::move(other.creation_func_)), object_(other.object_), is_created_(other.is_created_) {}

  shared_factory_impl(shared_factory_impl&& other) noexcept
      : creation_func_(std::move(other.creation_func_)), object_(std::move(other.object_)), is_created_(other.is_created_) {}

  shared_factory_impl& operator=(shared_factory_impl&& other) noexcept {
    object_ = std::move(other.object_);
    return *this;
  }

  template <class TInjector, class TDependency>
  auto operator()(const TInjector& const_injector, const TDependency&) {
    if (!is_created_) {
      auto& injector = const_cast<TInjector&>(const_injector);
      auto rebound_injector = injector_rebinder<typename TDependency::expected, T>{}.rebind(injector);

      std::shared_ptr<T> object;
#ifdef __EXCEPTIONS
      try {
        object = creation_func_(rebound_injector);
      } catch (...) {
        injector = std::move(rebound_injector);
        throw;
      }
#else
      object = creation_func_(rebound_injector);
#endif
      injector = std::move(rebound_injector);

      //<<should be saved after move otherwise will be overriden>>
      object_ = object;
      is_created_ = true;
    }
    return object_;
  }

 private:
  TFunc&& creation_func_;
  std::shared_ptr<T> object_;
  bool is_created_;
};

template <class T, class TFunc>
auto shared_factory(TFunc&& creation_func) {
  return shared_factory_impl<T, TFunc>(std::move(creation_func));
}

template <class T, class TFunc>
auto conditional_shared_factory(TFunc condition_func) {
  return shared_factory<T>([&](const auto& injector) {
    std::shared_ptr<T> object;
    if (condition_func()) {
      object = injector.template create<std::shared_ptr<T>>();
    }
    return object;
  });
}

//<-
struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy() = 0;
};

struct implementation : interface {
  static auto& ctor_calls() {
    static auto calls = 0;
    return calls;
  }
  static auto& dtor_calls() {
    static auto calls = 0;
    return calls;
  }
  implementation() { ctor_calls()++; }
  ~implementation() { dtor_calls()++; }
  void dummy() override {}
};

struct implementation_exception : interface {
#ifdef __EXCEPTIONS
  implementation_exception() { throw 0; }
#endif
  void dummy() override {}
};

//->

int main() {
  assert(!implementation::ctor_calls());
  assert(!implementation::dtor_calls());

  {
    //<<define injector>>
    // clang-format off
    auto injector = di::make_injector(
	  di::bind<implementation_exception>().to(shared_factory<implementation_exception>([&](const auto& inner_injector)
      {
        //<<throws an exception here>>
        return inner_injector.template create<std::shared_ptr<implementation_exception>>();
      })),
      //<<bind factory interface to implementation>>
      di::bind<implementation>().to(shared_factory<implementation>([&](const auto& inner_injector)
      {
        static int calls = 0;
        assert(1 == ++calls);
        //<<shouldn't be recursive call here>>
        return inner_injector.template create<std::shared_ptr<implementation>>();
      })),
      di::bind<interface>().to(conditional_shared_factory<implementation>([&]()
      {
        static int calls = 0;
        assert(1 == ++calls);
        return true;
      }))
    );
    // clang-format on

    //<<create `implementation` via `shared_factory`>>
    auto i1 = injector.create<std::shared_ptr<implementation>>();
    auto i2 = injector.create<std::shared_ptr<implementation>>();
    assert(i1);
    assert(i1 == i2);
    assert(1 == implementation::ctor_calls());
    assert(!implementation::dtor_calls());

    //<<create `interface` via `conditional_shared_factory`>>
    auto i3 = injector.create<std::shared_ptr<interface>>();

//<<make sure that exception doesn't affect injector>>
#ifdef __EXCEPTIONS
    bool exception_thrown = false;
    try {
      injector.create<std::shared_ptr<implementation_exception>>();
    } catch (...) {
      exception_thrown = true;
    }
    assert(exception_thrown);
#endif

    auto i4 = injector.create<std::shared_ptr<interface>>();
    assert(i3);
    assert(i3 == i4);
    assert(1 == implementation::ctor_calls());
    assert(!implementation::dtor_calls());
  }
  //<<Destroyed with injector>>
  assert(1 == implementation::dtor_calls());
}
