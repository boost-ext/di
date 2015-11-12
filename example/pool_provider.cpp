//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[pool_provider
//<-
#include <cassert>
#include <memory>
//->

#include <boost/di.hpp>

//<-
namespace boost {
namespace di {
inline namespace v1 {
namespace wrappers {

template <class T, class TDeleter>
struct unique<std::unique_ptr<T, TDeleter>> {
  template <class I>
  inline operator I() const noexcept {
    return *object;
  }

  template <class I>
  inline operator I*() noexcept {
    return object.release();
  }

  template <class I>
  inline operator const I*() noexcept {
    return object.release();
  }

  template <class I>
  inline operator std::shared_ptr<I>() noexcept {
    return {object.release(), object.get_deleter()};
  }

  template <class I, class D>
  inline operator std::unique_ptr<I, D>() noexcept {
    return static_cast<std::unique_ptr<T, TDeleter>&&>(object);
  }

  std::unique_ptr<T, TDeleter> object;
};
}
}
}
}  // boost::di::v1::wrappers
//->

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
};
struct implementation : interface {};
//->

/*<example `pool allocator`>*/
template <class T>
struct pool_allocator {
  static T* allocate();
  static void deallocate(T*);
};

//<-
template <class T>
T* pool_allocator<T>::allocate() {
  return (T*)::operator new(sizeof(T));
}

template <class T>
void pool_allocator<T>::deallocate(di::aux::owner<T*> ptr) {
  return ::operator delete(ptr);
}

template <class T>
struct pool_deleter {
  void operator()(di::aux::owner<T*> ptr) const noexcept { pool_allocator<T>::deallocate(ptr); }
};
//->

/*<define `example` class as usual>*/
struct example {
  explicit example(int i, std::unique_ptr<interface, pool_deleter<interface>> up, std::shared_ptr<interface> sp) {
    assert(i == 42);
    assert(dynamic_cast<implementation*>(up.get()));
    assert(dynamic_cast<implementation*>(sp.get()));
  }
};

/*<define `pool provider`>*/
struct pool_provider {
  template <class...>
  struct is_creatable {
    static constexpr auto value = true;
  };

  template <class T, class TInitialization, class TMemory, class... TArgs>
  auto get(const TInitialization&  // direct/uniform
           ,
           const TMemory&  // stack/heap
           ,
           TArgs&&... args) const {
    auto memory = pool_allocator<T>::allocate();
    return std::unique_ptr<T, pool_deleter<T>>{new (memory) T(std::forward<TArgs>(args)...)};
  }
};

/*<override `di` provider configuration>*/
class config : public di::config {
 public:
  template <class T>
  static auto provider(const T&) noexcept {
    return pool_provider{};
  }
};

int main() {
  /*<<make injector with simple configuration>>*/
  auto injector = di::make_injector<config>(di::bind<int>().to(42), di::bind<interface>().to<implementation>());

  /*<<create `example` using configuration with `pool_provider`>>*/
  injector.create<example>();
}

//]
