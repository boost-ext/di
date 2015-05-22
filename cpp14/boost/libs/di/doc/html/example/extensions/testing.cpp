//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[testing
//<-
#include <cassert>
#include <memory>
#include <map>
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

/*<<`mocks provider` configuration>>*/
template<class TInjector = di::_>
class mocks_provider_impl : public di::config {
    class not_implemented : public std::exception { };

    class expectations : public std::map<std::type_index, std::function<std::shared_ptr<void>()>> {
    public:
        template<class T>
        void will_return(T value) {
            operator [](current) = [=]{ return std::make_shared<T>(value); };
        }

        void add(std::type_index type, std::function<std::shared_ptr<void>()> call) {
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
                    return *(int*)it->second().get();
                }

                throw not_implemented{};
            }

        private:
            const expectations& expectations_;
        };

        struct not_resolved { };

        template<class T>
        using is_resolvable = std::integral_constant<
            bool
          , !std::is_same<
                not_resolved
              , decltype(di::core::binder::resolve<T, di::no_name, not_resolved>((TInjector*)nullptr))
            >{}
        >;

        template<
            class I
          , class T
          , class TInitialization
          , class TMemory
          , class... TArgs
          , std::enable_if_t<is_resolvable<I>{} || !std::is_polymorphic<T>{}, int> = 0
        > auto get(const TInitialization&
                 , const TMemory&
                 , TArgs&&... args) const {
            return new T{std::forward<TArgs>(args)...};
        }

        template<
            class I
          , class T
          , class TInitialization
          , class TMemory
          , class... TArgs
          , std::enable_if_t<!is_resolvable<I>{} && std::is_polymorphic<T>{}, int> = 0
        > auto get(const TInitialization&
                 , const TMemory&
                 , TArgs&&...) const {
            return reinterpret_cast<T*>(new mock<T>{expectations_});
        }

        template<class...>
        struct is_creatable {
            static constexpr auto value = true;
        };

        const expectations& expectations_;
    };

public:
    explicit mocks_provider_impl(const TInjector& injector)
        : injector_(injector)
    { }

    auto provider() const noexcept {
        return mock_provider{expectations_};
    }

    /*<<extend injector functionality of call operator>>*/
    template<class R, class T, class... TArgs>
    expectations& operator()(R(T::*)(TArgs...)) {
        expectations_.add(std::type_index(typeid(T))
                        , []{ throw not_implemented{}; return nullptr; }
        );
        return expectations_;
    }

    /*<<extend injector functionality of conversion operator>>*/
    template<class T>
    operator T() const {
        return injector_.template create<T>();
    }

private:
    expectations expectations_;
    const TInjector& injector_;
};

using mocks_provider = mocks_provider_impl<>;

struct test {
    template<class Test>
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
    auto mi = di::make_injector<mocks_provider>();
    /*<<set expectations>>*/
    mi(&i1::get).will_return(42);
    mi(&i2::get).will_return(123);
    /*<<create object to test with interfaces to be injected by di and int value passed directly to constructor>>*/
    c object{mi, mi, 87};
};

test integration_test = [] {
    struct impl1 : i1 {
        int get() override { return 42; }
    };
    /*<<create injector with `mocks_provider`>>*/
    auto mi = di::make_injector<mocks_provider>(
        di::bind<int>.to(87) // custom value
      , di::bind<i1, impl1>  // original implementation
    );
    /*<<set expectations>>*/
    mi(&i2::get).will_return(123); // fake
    /*<<create object to test with mocked `i1` and original `i2` and injected int value>>*/
    mi.create<c>();
};

/*<<`mock_provider` is just a simple mock framework - to check out di integration with full mock support framework see: [@https://github.com/krzysztof-jusiak/mocks_injector]>>*/
int main() { }

//]

