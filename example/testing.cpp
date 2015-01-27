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
#include <map>
#include <typeindex>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct interface {
    virtual ~interface() noexcept = default;
    virtual int get_value() = 0;
};

class mocks_provider : public di::config {
    class expectations : public std::map<std::type_index, std::function<void*()>> {
    public:
        template<class T>
        void will_return(T value) {
            operator [](current) = [=]{ return (void*)&value; };
        }

        void add(std::type_index type, std::function<void*()> call) {
            current = type;
            operator [](current) = call;
        }

    private:
        std::type_index current{typeid(nullptr)};
    };

    struct mock_provider {
        template<class T>
        class mock {
        public:
            explicit mock(const expectations& exp)
                : expectations_(exp)
            { }

            virtual ~mock() noexcept = default;

            virtual int _1() {
                auto it = expectations_.find(std::type_index(typeid(T)));
                if (it != expectations_.end()) {
                    return *(int*)it->second();
                }

                throw 0;
            }

        private:
            const expectations& expectations_;
        };

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
            return reinterpret_cast<T*>(new mock<T>{expectations_});
        }

        const expectations& expectations_;
    };

public:
    auto provider() const noexcept {
        return mock_provider{expectations_};
    }

    template<class R, class T, class... TArgs>
    expectations& operator()(R(T::*ptr)(TArgs...)) {
        expectations_.add(std::type_index(typeid(T)), []{ throw 0; return nullptr; });
        return expectations_;
    }

private:
    expectations expectations_;
};

struct c {
    c(std::shared_ptr<interface> sp, int i) {
        assert(sp->get_value() == 42);
        assert(i == 87);
    }
};

template<class T, class F>
decltype(auto) expect_call(T& t, const F& f) {
    return t.config()(f);
}

struct test {
    template<class Test>
    test(const Test& test) {
        test();
    }
};

//test unit_test = [] {
    //auto _ = di::make_injector<mocks_provider>();

    //expect_call(_, &interface::get_value).will_return(42);
    //c sut{_, 87};
//};

//test integration_test = [] {
    //struct implementation : interface {
        //int get_value() override { return 42; }
    //};

    //auto mi = di::make_injector<mocks_provider>(
        //di::bind<int>.to(87)
      //, di::bind<interface, implementation>
    //);

    //mi.create<c>();
//};

int main() { }

//]

