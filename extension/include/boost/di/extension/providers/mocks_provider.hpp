//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
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

class expectations : public std::unordered_map<std::type_index, std::function<std::shared_ptr<void>()>> {
 public:
  template <class T>
  void will_return(T value) {
    operator[](current) = [=] { return std::make_shared<T>(value); };
  }

  void add(std::type_index type, std::function<std::shared_ptr<void>()> call) {
    current = type;
    operator[](current) = call;
  }

 private:
  std::type_index current{typeid(nullptr)};
};

/*<<`mocks provider` configuration>>*/
class mocks_provider : public config {
  template <class TInjector>
  struct mock_provider {
    template <class T>
    class mock {
     public:
      explicit mock(const extension::expectations& exp) : expectations_(exp) {}
      virtual ~mock() noexcept = default;

      virtual int _1() {
        auto it = expectations_.find(std::type_index(typeid(T)));
        if (it != expectations_.end()) {
          return *(int*)it->second().get();
        }

        assert(false && "not implemented");
        return {};
      }

     private:
      const extension::expectations& expectations_;
    };

    template <class>
    static std::false_type resolve_impl(...);

    template <class TKey>
    static std::true_type resolve_impl(TKey*);

    template <class TKey, class T>
    struct resolve : decltype(resolve_impl<TKey>(aux::declval<T*>())) {};

    template <class>
    struct transform;

    template <class T>
    struct given {
      using type = typename T::given;
    };

    template <class... Ts>
    struct map : Ts... {};

    template <class... Ts>
    struct transform<aux::type_list<Ts...>> : map<aux::type<typename given<Ts>::type>...> {};

    template <class T>
    using is_resolvable = resolve<aux::type<T>, transform<typename TInjector::deps>>;

    template <class T, class TInitialization, class TMemory, class... TArgs>
    std::enable_if_t<is_resolvable<T>::value || !std::is_polymorphic<T>::value, T*> get(const TInitialization&, const TMemory&,
                                                                                        TArgs&&... args) const {
      return new T{std::forward<TArgs>(args)...};
    }

    template <class T, class TInitialization, class TMemory, class... TArgs>
    std::enable_if_t<!is_resolvable<T>::value && std::is_polymorphic<T>::value, T*> get(const TInitialization&, const TMemory&,
                                                                                        TArgs&&...) const {
      return reinterpret_cast<T*>(new mock<T>{expectations_});
    }

    template <class...>
    struct is_creatable {
      static constexpr auto value = true;
    };

    const extension::expectations& expectations_;
  };

 public:
  template <class TInjector>
  auto provider(const TInjector*) noexcept {
    return mock_provider<TInjector>{expectations_};
  }

  auto& expectations() { return expectations_; }

 private:
  extension::expectations expectations_;
};

template <class TInjector>
struct mocks : TInjector {
  using TInjector::TInjector;

  template <class T>
  operator T() const noexcept {
    return this->template create<T>();
  }
};

template <class... TDeps>
auto mocks_injector(TDeps... args) noexcept {
  auto injector = make_injector<mocks_provider>(args...);
  using type = decltype(injector);
  return mocks<type>{static_cast<type&&>(injector)};
}

template <class TInjector, class R, class T, class... TArgs>
auto& expect(TInjector& injector, R (T::*)(TArgs...)) {
  auto& expectations = ((di::core::injector__<TInjector>&)injector).cfg().expectations();
  expectations.add(std::type_index(typeid(T)), [] {
    assert(false && "not implemented");
    return nullptr;
  });
  return expectations;
}

}  // namespace extension
BOOST_DI_NAMESPACE_END
