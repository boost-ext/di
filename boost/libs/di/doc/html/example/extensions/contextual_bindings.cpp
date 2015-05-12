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
//->
#define BOOST_DI_CFG_NO_PREPROCESSED_HEADERS
#include <boost/di.hpp>

namespace di = boost::di;

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

template<class>
struct get_type;

template<class T>
struct get_type<di::aux::type<T>> {
    using type = T;
};

class build_types : public di::config {
public:
    auto policies() noexcept {
        return di::make_policies([&](auto type, auto dependency, auto... ctor) {
            using arg = typename decltype(type)::type;
            types[v[i - 1]].push_back(&typeid(arg));
            auto ctor_size = sizeof...(ctor);
            while (ctor_size--) {
                v.insert((v.begin()+i), v[i-1]+1);
            }
            ++i;
        });
    }

    std::map<int, std::vector<const std::type_info*>> types;

private:
    std::vector<int> v = { 0 };
    int i = 1;
};

template<class>
struct type_;

template<class... Ts>
struct type_<di::aux::type_list<Ts...>> {
    using element_type = di::aux::type_list<Ts...>;

    template<class T>
    auto type() {
        return std::make_unique<type_<di::aux::type_list<Ts..., T>>>();
    }
};

template<class T>
struct type {
    using element_type = di::aux::type_list<T>;

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

template<class T, class R>
struct all {
    using type = R;
    T object;
};

template<class R>
struct then_ {
    template<class T>
    auto then(T value) {
        return all<T, R>{value};
    }
};

template<class T>
then_<typename T::element_type> when(T&&) {
    return {};
}

template<class TResult>
struct contextual {
    template<class... Ts>
    explicit contextual(Ts&&... args)
        : types{std::make_pair(create_types(typename Ts::type{}), args.object)...}
    { }

    template<class... Ts>
    std::vector<const std::type_info*> create_types(const di::aux::type_list<Ts...>&) const {
        return {&typeid(Ts)...};
    }

    template<class TInjector>
    auto operator()(const TInjector& injector) const -> TResult {
        const auto& call_stack = injector.types;

        for (const auto& el : types) {
            auto level = 0;
            auto found = true;
            for (const auto& t : el.first) {
                const auto& cs = call_stack.at(level);
                if (std::find(cs.begin(), cs.end(), t) == cs.end()) {
                    found = false;
                    break;
                }
                ++level;
            }

            if (found) {
                return el.second;
            }
        }

        return {};
    }

    std::vector<std::pair<std::vector<const std::type_info*>, TResult>> types;
};

int main() {
    auto injector = di::make_injector<build_types>(
        di::bind<int>.when(context = type<a>() -> type<b>() -> type<c>()).then(3)
                     .when(context = type<a>() -> type<b>()).then(2)
                     .when(context = type<a>()).then(1)
        )
    );

            }
    injector.create<a>();
}

//]

