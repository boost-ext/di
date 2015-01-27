//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[testing
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct interface {
    virtual ~interface() noexcept = default;
    virtual int get() = 0;
};

class mocks_provider : public di::config {
    struct dummy_implementation {
        virtual ~dummy_implementation() noexcept = default;
        virtual int f1() {
            return 42;
        }
    };

    struct mock_provider {
        template<class, class T, class TInitialization, class TMemory, class... TArgs, std::enable_if_t<!std::is_polymorphic<T>{}, int> = 0>
        auto get(const TInitialization&
               , const TMemory&
               , TArgs&&... args) const {
            return new T(std::forward<TArgs>(args)...);
        }

        template<class, class T, class TInitialization, class TMemory, class... TArgs, std::enable_if_t<std::is_polymorphic<T>{}, int> = 0>
        auto get(const TInitialization&
               , const TMemory&
               , TArgs&&... args) const {
            return reinterpret_cast<T*>(new dummy_implementation{});
        }
    };

public:
    auto provider() const noexcept {
        return mock_provider{};
    }
};

struct c {
    c(std::shared_ptr<interface> i) {
        assert(i->get() == 42);
    }
};

int main() {
    //auto injector = di::make_injector<mocks_provider>();
    //injector.create<c>();
}

//]

