//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
};
struct implementation : interface {};
//->

/*<define `example` class as usual>*/
struct example {
  explicit example(int i, std::unique_ptr<interface> up) {
    assert(i == 42);
    assert(dynamic_cast<implementation*>(up.get()));
  }
};

/*<define `custom provider`>*/
struct custom_provider {
  template <class...>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::direct&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T(std::forward<TArgs>(args)...);
  }

  template <class T, class TMemory, class... TArgs>
  auto get(const di::type_traits::uniform&, const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    return new T{std::forward<TArgs>(args)...};
  }
};

/*<override `di` provider configuration>*/
class config : public di::config {
 public:
  static auto provider(...) noexcept { return custom_provider{}; }
};

int main() {
  /*<<make injector with simple configuration>>*/
  // clang-format off
  auto injector = di::make_injector<config>(
    di::bind<int>().to(42)
  , di::bind<interface>().to<implementation>()
  );
  // clang-format on

  /*<<create `example` using `custom_provider`>>*/
  injector.create<example>();
}
