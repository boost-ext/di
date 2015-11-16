//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXPOSED_HPP
#define BOOST_DI_SCOPES_EXPOSED_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"
#include "boost/di/fwd.hpp"

namespace scopes {

template <class TScope>
class exposed {
 public:
  template <class TExpected, class TGiven>
  class scope {
#if defined(__GNUC__) || defined(_MSC_VER)  // __pph__
    using type = aux::conditional_t<aux::is_copy_constructible<TExpected>::value, TExpected, TExpected*>;
#else   // __pph__
    using type = TExpected;
#endif  // __pph__

    struct iprovider {
      TExpected* (*heap)(const iprovider*) = nullptr;
      type (*stack)(const iprovider*) = nullptr;

      auto get(const type_traits::heap& = {}) const noexcept { return ((iprovider*)(this))->heap(this); }
      auto get(const type_traits::stack&) const noexcept { return ((iprovider*)(this))->stack(this); }
    };

    template <class TInjector>
    struct provider_impl {
      TExpected* (*heap)(const provider_impl*) = nullptr;
      type (*stack)(const provider_impl*) = nullptr;

      template <class T>
      static T create(const provider_impl* object) noexcept {
        return static_cast<const core::injector__<TInjector>&>(object->injector).create_impl(aux::type<T>{});
      }

      template <class T>
      static T create_successful(const provider_impl* object) noexcept {
        return static_cast<const core::injector__<TInjector>&>(object->injector).create_successful_impl(aux::type<T>{});
      }

      explicit provider_impl(const TInjector& injector) noexcept
          : provider_impl(
                injector,
                aux::integral_constant<bool, core::injector__<TInjector>::template is_creatable<TExpected*>::value>{},
                aux::integral_constant<bool, core::injector__<TInjector>::template is_creatable<TExpected>::value>{}) {}

      provider_impl(const TInjector& injector, const aux::true_type&, const aux::true_type&) noexcept
          : heap(&provider_impl::template create_successful<TExpected*>),
            stack(&provider_impl::template create_successful<type>),
            injector(injector) {}

      provider_impl(const TInjector& injector, const aux::false_type&, const aux::true_type&) noexcept
          : stack(&provider_impl::template create_successful<type>),
            injector(injector) {}

      provider_impl(const TInjector& injector, const aux::true_type&, const aux::false_type&) noexcept
          : heap(&provider_impl::template create_successful<TExpected*>),
            injector(injector) {}

      provider_impl(const TInjector& injector, const aux::false_type&, const aux::false_type&)
          : heap(&provider_impl::template create<TExpected*>)  // creatable constraint not satisfied
            ,
            injector(injector) {}

      TInjector injector;
    };

   public:
    template <class>
    using is_referable = aux::false_type;

    template <class TInjector, BOOST_DI_REQUIRES(aux::is_injector<TInjector>::value) = 0>
    explicit scope(const TInjector& injector) noexcept {
      static auto provider = provider_impl<TInjector>{injector};
      provider.injector = injector;
      provider_ = (iprovider*)&provider;
    }

    template <class T, class TProvider>
    static T try_create(const TProvider&);

    template <class T, class TProvider>
    auto create(const TProvider&) {
      return scope_.template create<T>(*provider_);
    }

   private:
    iprovider* provider_ = nullptr;
    typename TScope::template scope<TExpected, TGiven> scope_;
  };
};

}  // scopes

#endif
