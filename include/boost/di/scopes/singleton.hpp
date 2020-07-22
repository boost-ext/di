//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SINGLETON_HPP
#define BOOST_DI_SCOPES_SINGLETON_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"  // type_traits::stack
#include "boost/di/wrappers/shared.hpp"

namespace scopes {

class singleton {
  template <class T, class = decltype(aux::has_shared_ptr__(aux::declval<T>()))>
  class scope_impl {
   public:
    template <class T_, class>
    using is_referable = typename wrappers::shared<singleton, T&>::template is_referable<T_>;

    template <class, class, class TProvider>
    static decltype(wrappers::shared<singleton, T&>{aux::declval<TProvider>().get(type_traits::stack{})}) try_create(
        const TProvider&);

    template <class, class, class TProvider>
    auto create(const TProvider& provider) {
      return create_impl(provider);
    }

   private:
    template <class TProvider>
    wrappers::shared<singleton, T&> create_impl(const TProvider& provider) {
      static auto object(provider.get(type_traits::stack{}));
      return wrappers::shared<singleton, T&>(object);
    }
  };

  template <class T>
  class scope_impl<T, aux::true_type> {
   public:
    template <class T_, class>
    using is_referable = typename wrappers::shared<singleton, T>::template is_referable<T_>;

    template <class, class, class TProvider, class T_ = aux::decay_t<decltype(aux::declval<TProvider>().get())>>
    static decltype(wrappers::shared<singleton, T_>{std::shared_ptr<T_>{std::shared_ptr<T_>{aux::declval<TProvider>().get()}}})
    try_create(const TProvider&);

    template <class, class, class TProvider>
    auto create(const TProvider& provider) {
      return create_impl<aux::decay_t<decltype(provider.get())>>(provider);
    }

   private:
    template <class T_, class TProvider>
    auto create_impl(const TProvider& provider) {
      static std::shared_ptr<T_> object{provider.get()};
      return wrappers::shared<singleton, T_, std::shared_ptr<T_>&>{object};
    }
  };

 public:
  template <class, class T>
  using scope = scope_impl<T>;
};

}  // namespace scopes

static constexpr __BOOST_DI_UNUSED scopes::singleton singleton{};

#endif
