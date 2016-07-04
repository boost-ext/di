//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct i1 {
  virtual ~i1() noexcept = default;
  virtual int get() = 0;
};

struct i2 {
  virtual ~i2() noexcept = default;
  virtual int get() = 0;
};

class expectations : public std::map<std::type_index, std::function<std::shared_ptr<void>()>> {
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
class mocks_provider : public di::config {
  template <class TInjector>
  struct mock_provider {
    template <class T>
    class mock {
     public:
      explicit mock(const expectations& exp) : expectations_(exp) {}
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
      const expectations& expectations_;
    };

    template <class>
    static std::false_type resolve_impl(...);

    template <class TKey>
    static std::true_type resolve_impl(TKey*);

    template <class TKey, class T>
    struct resolve : decltype(resolve_impl<TKey>((T*)0)) {};

    template <class>
    struct transform;

    template <class T>
    struct given {
      using type = typename T::given;
    };

    template <class... Ts>
    struct map : Ts... {};

    template <class... Ts>
    struct transform<di::aux::type_list<Ts...>> : map<di::aux::type<typename given<Ts>::type>...> {};

    template <class T>
    using is_resolvable = resolve<di::aux::type<T>, transform<typename TInjector::deps>>;

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

    const expectations& expectations_;
  };

 public:
  template <class TInjector>
  static auto provider(const TInjector*) noexcept {
    return mock_provider<TInjector>{get_expectations()};
  }

  static expectations& get_expectations() {
    static expectations expectations_;
    return expectations_;
  }
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
  auto injector = di::make_injector<mocks_provider>(args...);
  using type = decltype(injector);
  return mocks<type>{static_cast<type&&>(injector)};
}

template <class TInjector, class R, class T, class... TArgs>
expectations& expect(TInjector&, R (T::*)(TArgs...)) {
  TInjector::config::get_expectations().add(std::type_index(typeid(T)), [] {
    assert(false && "not implemented");
    return nullptr;
  });
  return TInjector::config::get_expectations();
}

struct test {
  template <class Test>
  test(const Test& test) {
    test();
  }
};
//->

struct c {
  c(std::shared_ptr<i1> sp, std::unique_ptr<i2> up, int i) {
    assert(sp->get() == 42);
    assert(up->get() == 123);
    assert(i == 87);
  }
};

/*<<define simple unit test>>*/
test unit_test = [] {
  /*<<create injector with `mocks_provider`>>*/
  auto mi = mocks_injector();

  /*<<set expectations>>*/
  expect(mi, &i1::get).will_return(42);
  expect(mi, &i2::get).will_return(123);

  /*<<create object to test with interfaces to be injected by di and int value passed directly to constructor>>*/
  c object{mi, mi, 87};
};

test integration_test = [] {
  struct impl1 : i1 {
    int get() override { return 42; }
  };

  /*<<create injector with `mocks_provider`>>*/
  // clang-format off
  auto mi = mocks_injector(
    di::bind<int>().to(87) // custom value
  , di::bind<i1>().to<impl1>() // original implementation
  );
  // clang-format on

  /*<<set expectations>>*/
  expect(mi, &i2::get).will_return(123);  // fake

  /*<<create object to test with mocked `i1` and original `i2` and injected int value>>*/
  mi.create<c>();
};

/*<<`mock_provider` is just a simple mock framework - to check out di integration with full mock support framework see:
 * [@https://github.com/krzysztof-jusiak/mocks_injector]>>*/
int main() {}
