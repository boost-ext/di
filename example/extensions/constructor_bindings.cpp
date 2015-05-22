//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[constructor_bindings
//<-
#include <cassert>
#include <memory>
#include <tuple>

struct interface { virtual ~interface() noexcept = default; virtual void dummy() = 0; };
struct implementation : interface { void dummy() override { } };

#include <boost/di/fwd.hpp>

/*<<to constructor dependency extension>>*/
struct to_constructor_ext {
    /*<<implementation of to_constructor extensions>>*/
    template<class T, class... TCtor>
    struct to_constructor_impl {
        template<class TInjector> // is_creatable
        auto operator()(const TInjector& injector) const  {
            return std::make_unique<T>(injector.template create<TCtor>()...);
        }
    };

    template<class>
    struct extension;

    /*<<extension implementation, add `to_constructor` method to dependency>>*/
    template<class TScope, class TExpected, class... Ts>
    struct extension<boost::di::core::dependency<TScope, TExpected, Ts...>> {
        template<class... TCtor>
        auto to_constructor() const noexcept {
            const auto& self = static_cast<const boost::di::core::dependency<TScope, TExpected, Ts...>&>(*this);
            return self.to(to_constructor_impl<TExpected, TCtor...>{});
        }
    };
};

//->

/*<<define dependency extensions>>*/
#define BOOST_DI_CFG_DEPENDENCY_EXTENSIONS ::to_constructor_ext

#include <boost/di.hpp>

namespace di = boost::di;

/*<<normal constructor - to_constructor binding is not needed for it>>*/
struct ctor {
    ctor(int i , std::string s, std::unique_ptr<interface> impl){
        assert(i == 2);
        assert(s == "hello");
        assert(dynamic_cast<implementation*>(impl.get()));
    }
};

/*<<variadic constructor - has to be `explicit`>>*/
struct variadic {
    template<class... Ts>
    explicit variadic(Ts&&... args) { // has to be explicit
        assert(sizeof...(Ts) == 3);
        std::tuple<Ts...> ts{std::forward<Ts>(args)...};
        assert(std::get<0>(ts) == 2);
        assert(std::get<1>(ts) == "hello");
        assert(dynamic_cast<implementation*>(std::get<2>(ts).get()));
    }
};

/*<<variadic arguments `c-style`>>*/
struct var_arg {
    explicit var_arg(int i, ...) {
        assert(i == 2);
    }
};

int main() {
    auto injector = di::make_injector(
        /*<<define constructor types>>*/
        di::bind<variadic>.to_constructor<int, std::string, std::unique_ptr<interface>>()
      , di::bind<ctor>.to_constructor<int, std::string, std::unique_ptr<interface>>()
      , di::bind<var_arg>.to_constructor<int, float, double>()

        /*<<additional bindings>>*/
      , di::bind<interface, implementation>
      , di::bind<int>.to(2)
      , di::bind<std::string>.to("hello")

    );

    /*<<create types using defined constructors>>*/
    injector.create<ctor>();
    injector.create<variadic>();
    injector.create<var_arg>();
}

//]

