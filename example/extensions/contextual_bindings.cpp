//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[contextual_bindings
//<-
#include <cassert>
#include <typeinfo>
#include <map>
#include <vector>
#include <memory>
//->

#define BOOST_DI_CFG_NO_PREPROCESSED_HEADERS
#include <boost/units/detail/utility.hpp>

#include <boost/di/fwd.hpp>
#include <boost/di/aux_/utility.hpp>

namespace di = boost::di;

template<class>
struct type_;

template<class... Ts>
struct type_<di::aux::type_list<Ts...>> {
    auto dump() const {
        std::vector<const std::type_info*> v;
        [](...){}((v.push_back(&typeid(Ts)), 0)...);
        return v;
    }

    template<class T>
    auto type() {
        return std::make_unique<type_<di::aux::type_list<Ts..., T>>>();
    }
};

template<class T>
struct type {
    auto dump() const {
        return std::vector<const std::type_info*>{&typeid(T)};
    }

    auto operator->() const {
        return std::make_unique<type_<di::aux::type_list<T>>>();
    }

    auto operator()() {
        return std::make_unique<type_<di::aux::type_list<T>>>();
    }
};

struct context_ {
    template<class T>
    auto operator=(std::unique_ptr<T> object) const {
        return *object;
    }

    template<class T>
    auto operator=(T object) const {
        return object;
    }
};

context_ context;

template<class TExpected>
struct contextual {
    template<class TInjector>
    auto operator()(const TInjector&) const -> TExpected {
        return {};
    }

    std::vector<std::pair<std::vector<const std::type_info*>, TExpected>> values_;
};

struct contextual_ext {
    template<class>
    struct extension;

    template<class TScope, class TExpected, class... Ts>
    struct extension<boost::di::core::dependency<TScope, TExpected, Ts...>> {
        template<class TContext>
        auto when(const TContext& context) noexcept {
            values_.push_back(std::make_pair(context.dump(), TExpected{}));
            auto& self = static_cast<boost::di::core::dependency<TScope, TExpected, Ts...>&>(*this);
            return self;
        }

        template<class T>
        auto then(T&& value) noexcept {
            values_.back().second = std::forward<T>(value);
            auto& self = static_cast<boost::di::core::dependency<TScope, TExpected, Ts...>&>(*this);
            return self;//.to(contextual<TExpected>{values_});
        }

        template<class T, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            return 42;
        }

        std::vector<std::pair<std::vector<const std::type_info*>, TExpected>> values_;
    };
};

/*<<define dependency extensions>>*/
#define BOOST_DI_CFG_DEPENDENCY_EXTENSIONS ::contextual_ext

#include <boost/di.hpp>

class build_types : public di::config {
public:
/*    auto policies() noexcept {*/
        //return di::make_policies([&](auto type, auto de, auto... ctor) {
            //using arg = typename decltype(type)::type;
            //types[v[i - 1]].push_back(&typeid(arg));
            //auto ctor_size = sizeof...(ctor);
            //while (ctor_size--) {
                //v.insert((v.begin()+i), v[i-1]+1);
            //}
            //++i;
        //});
    /*}*/

    std::map<int, std::vector<const std::type_info*>> types;

private:
    //std::vector<int> v = { 0 };
    //int i = 1;
};

struct c {
    c(int i) {
        assert(i == 3);
    }
};

struct b {
    b(c, int i) {
        assert(i == 2);
    }
};

struct a {
    a(b, int i) {
        assert(i == 1);
    }
};

int main() {
    //auto injector = di::make_injector<build_types>(
    auto injector = di::make_injector(
        di::bind<int>().when(context = type<a>() -> type<b>() -> type<c>()).then(3)
                     .when(context = type<a>() -> type<b>()).then(2)
                     .when(context = type<a>()).then(1)
    );

    injector.create<a>();
}

//]

