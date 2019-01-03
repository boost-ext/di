//
// Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <fruit/fruit.h>

struct I {
  virtual ~I() noexcept = default;
  virtual void dummy() = 0;
};
struct Impl : I {
  INJECT(Impl()) {}
  void dummy() override {}
};

fruit::Component<I> module() {
  return fruit::createComponent();
  //.bind<I, Impl>();
}

int main() {
  fruit::Injector<I> injector{module()};
  std::unique_ptr<I> object{injector.get<I*>()};
  object->dummy();
}

// clang-format off
/**
  *  In file included from fruit.cpp:1:
  *  In file included from .fruit/include/fruit/fruit.h:25:
  * fruit/include/fruit/impl/injection_errors.h:33:3: error: static_assert failed
  * "No explicit binding nor C::Inject definition was found for T."
  *  static_assert(AlwaysFalse<T>::value,
  * ^             ~~~~~~~~~~~~~~~~~~~~~
  * fruit/include/fruit/impl/component.defn.h:41:9: note: in instantiation of
  * 'fruit::impl::NoBindingFoundError<I>' requested here
  * (void)typename fruit::impl::meta::CheckIfError<typename Op::Result>::type();
  * ^
  *  fruit.cpp:6:12: note: in instantiation of function template specialization
  *  'fruit::Component<I>::Component<
  *    fruit::impl::meta::ConsComp<
  *    fruit::impl::meta::Vector<>*,
  *    fruit::impl::meta::Vector<>,
  *    fruit::impl::meta::Vector<>, 
  *    fruit::impl::meta::Vector<>,
  *    fruit::impl::meta::Vector<>
  *    >*>' requested here
  *  return fruit::createComponent();
  */
// clang-format on
