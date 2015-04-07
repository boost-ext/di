//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
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

namespace di = boost::di;

//<-
struct interface { virtual ~interface() noexcept = default; };
struct implementation : interface { };
//->

/*<example `pool allocator`>*/
template<class T>
struct pool_allocator {
    static T* allocate();
    static void deallocate(T*);
};

//<-
template<class T>
T* pool_allocator<T>::allocate() {
    return (T*)::operator new(sizeof(T));
}

template<class T>
void pool_allocator<T>::deallocate(T* ptr) {
    return ::operator delete(ptr);
}

template<class T>
struct pool_deleter {
    void operator()(T* ptr) const noexcept {
        pool_allocator<T>::deallocate(ptr);
    }
};
//->

/*<define `example` class as usual>*/
struct example {
    explicit example(int i, std::unique_ptr<interface, pool_deleter<interface>> up) {
        assert(i == 42);
        assert(dynamic_cast<implementation*>(up.get()));
    }
};

/*<define `pool provider`>*/
struct pool_provider {
    template<class...>
    struct is_creatable {
        static constexpr auto value = true;
    };

    template<class I, class T, class TInitialization, class TMemory, class... TArgs>
    auto get(const TInitialization& // direct/uniform
           , const TMemory& // stack/heap
           , TArgs&&... args) const {
        auto memory = pool_allocator<T>::allocate();
        return std::unique_ptr<T, pool_deleter<I>>{
            new (memory) T(std::forward<TArgs>(args)...)
        };
    }
};

/*<override `di` provider configuration>*/
class config : public di::config {
public:
    auto provider() const noexcept {
        return pool_provider{};
    }
};

int main() {
    /*<<make injector with simple configuration>>*/
    auto injector = di::make_injector<config>(
        di::bind<int>.to(42)
      , di::bind<interface, implementation>
    );

    /*<<create `example` using configuration with `pool_provider`>>*/
    injector.create<example>();
}

//]

