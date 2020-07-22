//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/wrappers/unique.hpp"

namespace scopes {

class unique {
 public:
  template <class, class>
  class scope {
   public:
    template <class...>
    using is_referable = aux::false_type;

    template <class T, class, class TProvider>
    static decltype(wrappers::unique<unique, decltype(aux::declval<TProvider>().get(
                                                 typename TProvider::config::template memory_traits<T>::type{}))>{
        aux::declval<TProvider>().get(typename TProvider::config::template memory_traits<T>::type{})})
    try_create(const TProvider&);

    template <class T, class, class TProvider>
    auto create(const TProvider& provider) const {
      using memory = typename TProvider::config::template memory_traits<T>::type;
      using wrapper = wrappers::unique<unique, decltype(provider.get(memory{}))>;
      return wrapper{provider.get(memory{})};
    }
  };
};

}  // namespace scopes

static constexpr __BOOST_DI_UNUSED scopes::unique unique{};

#endif
