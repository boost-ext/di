//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

namespace detail {
template <class, class TScope>
class session {
  class session_guard {
   public:
    explicit session_guard(bool& guard) : guard_(guard) { guard = true; }
    ~session_guard() { guard_ = false; }

   private:
    bool& guard_;
  };

 public:
  template <class TExpected, class TGiven>
  class scope {
    using scope_type = typename TScope::template scope<TExpected, TGiven>;

   public:
    template <class T, class TConfig>
    using is_referable = typename scope_type::template is_referable<T, TConfig>;

    template <class T, class TName, class TProvider>
#if defined(__MSVC__)
    static auto try_create(const TProvider& provider) -> T;
#else
    static auto try_create(const TProvider& provider) -> decltype(scope_type{}.template try_create<T, TName>(provider));
#endif

    template <class T, class TName, class TProvider>
    auto create(const TProvider& provider) {
      static std::shared_ptr<TGiven> null{nullptr};
      return get_session() ? scope_.template create<T, TName>(provider) : null;
    }

   private:
    scope_type scope_;
  };

  auto operator()() const { return session_guard{get_session()}; }

 private:
  static auto& get_session() {
    static auto is_in_session = false;
    return is_in_session;
  }
};
}  // namespace detail

template <class TName, class TScope = scopes::singleton>
auto session(const TName&, const TScope& = {}) {
  return detail::session<TName, TScope>{};
}

}  // namespace extension
BOOST_DI_NAMESPACE_END
