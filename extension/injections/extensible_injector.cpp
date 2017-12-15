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

template <class TDependency>
class dependency_proxy
    : di::core::dependency_base,
      public di::core::dependency_impl<di::core::dependency_concept<typename TDependency::expected, typename TDependency::name>,
                                       dependency_proxy<TDependency>> {
 public:
  using scope = typename TDependency::scope;
  using expected = typename TDependency::expected;
  using given = typename TDependency::given;
  using name = typename TDependency::name;
  using priority = typename TDependency::priority;

  dependency_proxy(TDependency& orig_dependency) noexcept : orig_dependency_(orig_dependency) {}

  dependency_proxy(dependency_proxy& other) noexcept : orig_dependency_(other.orig_dependency_) {}
  dependency_proxy(dependency_proxy&& other) noexcept : orig_dependency_(other.orig_dependency_) {}

 protected:
  using scope_t = typename scope::template scope<expected, given>;

  template <class T>
  using is_referable = typename scope_t::template is_referable<T>;

  template <class T, class TName, class TProvider>
  static decltype(scope_t::template try_create<T, TName>(di::aux::declval<TProvider>())) try_create(const TProvider&);

  template <class T, class TName, class TProvider>
  auto create(const TProvider& provider) {
    return static_cast<di::core::dependency__<TDependency>&>(orig_dependency_).template create<T, TName>(provider);
  }

 private:
  TDependency& orig_dependency_;
};

template <class TDependency, class TInjector>
auto make_extensible_impl(const di::aux::type<TDependency>&, TInjector& injector) {
  auto& dependency = di::core::binder::resolve<typename TDependency::expected, typename TDependency::name>(&injector);
  return dependency_proxy<TDependency>{dependency};
}

template <class... TDeps, class TInjector>
auto make_extensible(const di::aux::type_list<TDeps...>&, TInjector& injector) {
  return di::make_injector(make_extensible_impl(di::aux::type<TDeps>{}, injector)...);
}

template <class TInjector>
auto make_extensible(TInjector& injector) {
  return make_extensible(typename TInjector::deps{}, injector);
}

//<-

struct interface {
  virtual ~interface() noexcept = default;
  virtual int num() = 0;
};

struct implementation1 : interface {
  int num() override { return 1; }
};

struct implementation2 : interface {
  int num() override { return 2; }
};

struct implementation3 : interface {
  int num() override { return 3; }
};

struct example {
  example(std::unique_ptr<interface> i, std::shared_ptr<implementation3> im3) : i_(std::move(i)), im3_(im3) {}

  std::shared_ptr<interface> i_;
  std::shared_ptr<interface> im3_;
};

//->

int main() {
  //<<create instance object>>
  auto im3_orig = std::make_shared<implementation3>();

  //<<define injector>>
  // clang-format off
  auto orig_injector = di::make_injector(
      //<<bind interface to implementation1>>
      di::bind<interface>().to<implementation1>().in(di::unique)

      //<<bind implementation3 to shared instance>>
      , di::bind<implementation3>().to(im3_orig));

  {
    //<<define extended injector>>
    auto extended_injector = di::make_injector(
        //<<make extensible injector from original injector>>
        make_extensible(orig_injector)

        //<<override bound interface to implementation2>>
        , di::bind<interface>().to<implementation2>().in(di::unique)[di::override]
    );
    // clang-format on

    //<<both injectors live together>>
    auto orig_example = orig_injector.create<example>();
    assert(1 == orig_example.i_->num());
    assert(3 == orig_example.im3_->num());

    auto extended_example = extended_injector.create<example>();
    assert(2 == extended_example.i_->num());
    assert(3 == extended_example.im3_->num());

    //<<both injectors share the same dependency>>
    assert(extended_example.im3_ == orig_example.im3_);
    assert(im3_orig == orig_example.im3_);
  }
  //<<after death of extended dependency original dependency is still alive>>
  assert(im3_orig == orig_injector.create<std::shared_ptr<implementation3>>());
}
