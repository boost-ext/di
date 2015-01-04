//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[custom_provider
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() { }; };
struct implementation : interface { };
//->

/*<define `example` class as usual>*/
struct example {
    explicit example(int i, std::unique_ptr<interface> up) {
        assert(i == 42);
        assert(dynamic_cast<implementation*>(up.get()));
    }
};

/*<define `custom provider`>*/
class custom_provider : public di::config {
public:
    template<class T, class TMemory, class... TArgs>
    auto* get(const di::type_traits::direct&
            , const TMemory& // stack/heap
            , TArgs&&... args) const {
        return new T(std::forward<TArgs>(args)...);
    }

    template<class T, class TMemory, class... TArgs>
    auto* get(const di::type_traits::uniform&
            , const TMemory& // stack/heap
            , TArgs&&... args) const {
        return new T{std::forward<TArgs>(args)...};
    }
};

int main() {
    /*<<make injector with simple configuration>>*/
    auto injector = di::make_injector<custom_provider>(
        di::bind<int>.to(42)
      , di::bind<interface, implementation>
    );

    /*<<create `example` using `custom_provider`>>*/
    injector.create<example>();
}

//]

