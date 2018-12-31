//
// Copyright (c) 2012-2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

class runtime_provider : public config {
  using bindings_t = std::unordered_map<std::type_index, std::function<void *()>>;

  class abstract_provider {
   public:
    explicit abstract_provider(const bindings_t &bindings) : bindings_{bindings} {}

    template <class...>
    struct is_creatable {
      static constexpr auto value = true;
    };

    template <class T, class TInitialization, class TMemory, class... TArgs>
    auto get(const TInitialization &, const TMemory &, TArgs &&... args) const {
      auto it = bindings_.find(std::type_index(typeid(T)));
      if (it == bindings_.end()) {
        return get<T>(std::integral_constant < bool,
                      !std::is_abstract<T>::value && std::is_constructible<T, TArgs...>::value > {},
                      std::forward<TArgs>(args)...);
      }
      return static_cast<T *>(it->second());
    }

   private:
    template <class T, class... TArgs>
    auto get(std::true_type, TArgs &&... args) const {
      return new T{std::forward<TArgs>(args)...};
    }

    template <class T, class... TArgs>
    auto get(std::false_type, TArgs &&...) const {
      assert(false && "Type not bound!");
      return static_cast<T *>(nullptr);
    }

    const bindings_t &bindings_;
  };

 public:
  auto provider(...) { return abstract_provider{bindings_}; }
  auto &bindings() { return bindings_; }

 private:
  bindings_t bindings_{};
};

class injector : public core::injector<runtime_provider> {
  template <class...>
  struct any;

  template <class TParent, class TInjector>
  struct any<core::any_type_fwd<TParent>, TInjector> {
    using type = core::any_type<TParent, TInjector>;
  };

  template <class TParent, class TInjector>
  struct any<core::any_type_ref_fwd<TParent>, TInjector> {
    using type = core::any_type_ref<TParent, TInjector>;
  };

  template <class TParent, class TInjector>
  struct any<core::any_type_1st_fwd<TParent>, TInjector> {
    using type = core::any_type_1st<TParent, TInjector>;
  };

  template <class TParent, class TInjector>
  struct any<core::any_type_1st_ref_fwd<TParent>, TInjector> {
    using type = core::any_type_1st_ref<TParent, TInjector>;
  };

 public:
  injector() : core::injector<runtime_provider>{core::init{}} {}

  template <class T>
  void install(const T &binding) {
    cfg().bindings()[std::type_index(typeid(typename T::expected))] = [this, &binding] {
      return make<typename T::given>(binding);
    };
  }

  template <class... TBindings>
  void install(const TBindings &... bindings) {
    (void)aux::swallow{0, (install(bindings), 0)...};
  }

 private:
  template <class T, class TBinding, std::enable_if_t<std::is_class<T>::value, int> = 0>
  auto make(const TBinding &) {
    return make<T>(typename ctor_traits<T>::type{});
  }

  template <class T, class TBinding, std::enable_if_t<!std::is_class<T>::value, int> = 0>
  auto make(const TBinding &binding) {
    return new T{binding.object_};
  }

  template <class T, class TInit, class... Ts>
  auto make(aux::pair<TInit, aux::type_list<Ts...>>) {
    return new T{typename any<Ts, injector>::type{*this}...};
  }
};

}  // namespace extension
BOOST_DI_NAMESPACE_END
