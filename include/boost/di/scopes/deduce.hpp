// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace scopes {

class deduce {
 public:
  template <class TExpected, class TGiven>
  class scope {
   public:
    template <class T, class TConfig>
    using is_referable =
        typename TConfig::template scope_traits<T>::type::template scope<TExpected, TGiven>::template is_referable<T, TConfig>;

    template <class T, class TName, class TProvider>
    static decltype(typename TProvider::config::template scope_traits<T>::type::template scope<TExpected, TGiven>{}
                        .template try_create<T, TName>(aux::declval<TProvider>()))
    try_create(const TProvider&);

    template <class T, class TName, class TProvider>
    auto create(const TProvider& provider) {
      using scope_traits = typename TProvider::config::template scope_traits<T>::type;
      using scope = typename scope_traits::template scope<TExpected, TGiven>;
      return scope{}.template create<T, TName>(provider);
    }
  };
};

}  // namespace scopes

static constexpr __BOOST_DI_UNUSED scopes::deduce deduce{};

#endif
