// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/wrappers/unique.hpp"

namespace scopes {

namespace detail {

template <class T, class TExpected, class TGiven>
struct arg {
  using type = T;
  using expected = TExpected;
  using given = TGiven;
};

template <class T>
struct wrapper_traits {
  using type = wrappers::unique<T>;
};

template <class T>
struct wrapper_traits<std::shared_ptr<T>> {
  using type = wrappers::shared<T>;
};

template <class T>
using wrapper_traits_t = typename wrapper_traits<T>::type;

template <class T>
class no_implicit_conversions : public T {
  template <class U>
  operator U() const;
};

BOOST_DI_HAS_TYPE(has_result_type, result_type);

template <class TGiven, class TProvider, class... Ts>
struct is_expr
    : aux::integral_constant<
          bool, aux::is_callable_with<TGiven, no_implicit_conversions<typename TProvider::injector_t>, Ts...>::value &&
                    !has_result_type<TGiven>::value> {};

}  // detail

class external {
 public:
  template <class, class TGiven, class = int>
  struct scope {
    template <class>
    using is_referable = aux::false_type;

    explicit scope(const TGiven& object) : object_{object} {}

    template <class, class TProvider>
    static wrappers::unique<TGiven> try_create(const TProvider&);

    template <class, class TProvider>
    auto create(const TProvider&) const noexcept {
      return wrappers::unique<TGiven>{object_};
    }

    TGiven object_;
  };

  template <class TExpected, class TGiven>
  struct scope<TExpected, std::shared_ptr<TGiven>> {
    template <class T>
    using is_referable = typename wrappers::shared<TGiven>::template is_referable<aux::remove_qualifiers_t<T>>;

    explicit scope(const std::shared_ptr<TGiven>& object) : object_{object} {}

    template <class, class TProvider>
    static wrappers::shared<TGiven> try_create(const TProvider&);

    template <class, class TProvider>
    auto create(const TProvider&) const noexcept {
      return wrappers::shared<TGiven>{object_};
    }

    std::shared_ptr<TGiven> object_;
  };

  template <class TExpected, class TGiven>
  struct scope<TExpected, std::initializer_list<TGiven>> {
    template <class>
    using is_referable = aux::false_type;

    scope(const std::initializer_list<TGiven>& object) : object_(object) {}

    template <class, class TProvider>
    static std::initializer_list<TGiven> try_create(const TProvider&);

    template <class, class TProvider>
    auto create(const TProvider&) const noexcept {
      return wrappers::unique<std::initializer_list<TGiven>>{object_};
    }

    std::initializer_list<TGiven> object_;
  };

  template <class TExpected, class TGiven>
  struct scope<TExpected, TGiven&, BOOST_DI_REQUIRES(!aux::is_callable<TGiven>::value)> {
    template <class>
    using is_referable = aux::true_type;

    explicit scope(TGiven& object) : object_{object} {}

    template <class, class TProvider>
    static wrappers::shared<TGiven&> try_create(const TProvider&);

    template <class, class TProvider>
    auto create(const TProvider&) const noexcept {
      return object_;
    }

    wrappers::shared<TGiven&> object_;
  };

  template <class TExpected, class TGiven>
  struct scope<TExpected, TGiven, BOOST_DI_REQUIRES(aux::is_callable<TGiven>::value)> {
    template <class>
    using is_referable = aux::false_type;

    explicit scope(const TGiven& object) : object_(object) {}

#if defined(_MSC_VER)  // __pph__
    template <class T, class TProvider>
    static T try_create(const TProvider&) noexcept;
#else   // __pph__

    template <class, class TProvider,
              BOOST_DI_REQUIRES(!detail::is_expr<TGiven, TProvider>::value && aux::is_callable<TGiven>::value &&
                                aux::is_callable<TExpected>::value) = 0>
    static wrappers::unique<TExpected> try_create(const TProvider&) noexcept;

    template <class T, class TProvider,
              BOOST_DI_REQUIRES(!detail::is_expr<TGiven, TProvider>::value && aux::is_callable_with<TGiven>::value &&
                                !aux::is_callable<TExpected>::value) = 0>
    static auto try_create(const TProvider&) noexcept
        -> detail::wrapper_traits_t<decltype(aux::declval<typename aux::identity<TGiven, T>::type>()())>;

    template <class, class TProvider, BOOST_DI_REQUIRES(detail::is_expr<TGiven, TProvider>::value) = 0>
    static detail::wrapper_traits_t<decltype(aux::declval<TGiven>()(aux::declval<typename TProvider::injector_t>()))>
    try_create(const TProvider&) noexcept;

    template <
        class T, class TProvider,
        BOOST_DI_REQUIRES(detail::is_expr<TGiven, TProvider, const detail::arg<T, TExpected, TGiven>&>::value) = 0>
    static detail::wrapper_traits_t<decltype(aux::declval<TGiven>()(aux::declval<typename TProvider::injector_t>(),
                                                                    aux::declval<detail::arg<T, TExpected, TGiven>>()))>
    try_create(const TProvider&) noexcept;
#endif  // __pph__

    template <class, class TProvider,
              BOOST_DI_REQUIRES(!detail::is_expr<TGiven, TProvider>::value && aux::is_callable<TGiven>::value &&
                                aux::is_callable<TExpected>::value) = 0>
    auto create(const TProvider&) const noexcept {
      return wrappers::unique<TExpected>{object_};
    }

    template <class T, class TProvider,
              BOOST_DI_REQUIRES(!detail::is_expr<TGiven, TProvider>::value && aux::is_callable_with<TGiven>::value &&
                                !aux::is_callable<TExpected>::value) = 0>
    auto create(const TProvider&) const noexcept {
      using wrapper = detail::wrapper_traits_t<decltype(aux::declval<TGiven>()())>;
      return wrapper{object_()};
    }

    template <class, class TProvider, BOOST_DI_REQUIRES(detail::is_expr<TGiven, TProvider>::value) = 0>
    auto create(const TProvider& provider) noexcept {
      using wrapper = detail::wrapper_traits_t<decltype((object_)(provider.injector_))>;
      return wrapper{(object_)(provider.injector_)};
    }

    template <
        class T, class TProvider,
        BOOST_DI_REQUIRES(detail::is_expr<TGiven, TProvider, const detail::arg<T, TExpected, TGiven>&>::value) = 0>
    auto create(const TProvider& provider) noexcept {
      using wrapper =
          detail::wrapper_traits_t<decltype((object_)(provider.injector_, detail::arg<T, TExpected, TGiven>{}))>;
      return wrapper{(object_)(provider.injector_, detail::arg<T, TExpected, TGiven>{})};
    }

    TGiven object_;
  };
};

}  // scopes

#endif
