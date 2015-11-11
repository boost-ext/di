//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SINGLETON_HPP
#define BOOST_DI_SCOPES_SINGLETON_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"  // type_traits::stack
#include "boost/di/wrappers/shared.hpp"

namespace scopes {

aux::false_type has_shared_ptr__(...);

template <class T>
auto has_shared_ptr__(T && ) -> aux::is_valid_expr<decltype(std::shared_ptr<T>{})>;

class singleton {
 public:
  template <class, class>
  class scope_impl;

  template <class, class T>
  using scope = scope_impl<T, decltype(has_shared_ptr__(aux::declval<T>()))>;

  template <class T>
  class scope_impl<T, aux::false_type> {
   public:
    template <class T_>
    using is_referable = typename wrappers::shared<T&>::template is_referable<T_>;

    template <class, class TProvider>
    static decltype(wrappers::shared<T&>{aux::declval<TProvider>().get(type_traits::stack{})}) try_create(
        const TProvider&);

    template <class, class TProvider>
    auto create(const TProvider& provider) {
      return create_impl(provider);
    }

   private:
    template <class TProvider>
    auto create_impl(const TProvider& provider) {
      static T object(provider.get(type_traits::stack{}));
      return wrappers::shared<T&>(object);
    }
  };

  template <class T>
  class scope_impl<T, aux::true_type> {
   public:
    template <class T_>
    using is_referable = typename wrappers::shared<T>::template is_referable<T_>;

    template <class, class TProvider, class T_ = aux::decay_t<decltype(aux::declval<TProvider>().get())>>
    static decltype(wrappers::shared<T_>{std::shared_ptr<T_>{std::shared_ptr<T_>{aux::declval<TProvider>().get()}}})
    try_create(const TProvider&);

    template <class T_, class TProvider>
    auto create(const TProvider& provider) {
      return create_impl<aux::decay_t<decltype(provider.get())>>(provider);
    }

   private:
    template <class T_, class TProvider>
    auto create_impl(const TProvider& provider) {
      static std::shared_ptr<T_> object{provider.get()};
      return wrappers::shared<T_, std::shared_ptr<T_>&>{object};
    }
  };
};

}  // scopes

#endif
