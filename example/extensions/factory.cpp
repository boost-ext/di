//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[factory
//<-
#include <cassert>
#include <memory>
#include <functional>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() noexcept = default; virtual void dummy() = 0; };
struct implementation : interface { void dummy() override { } };

constexpr di::no_name assisted{};

template<class T>
class factory {
public:
    template<class TInjector>
    auto operator()(const TInjector& injector) const {
        return [&](auto&&... args) {
            using ctor = typename di::type_traits::ctor_traits<T>::type;
            return this->create(injector, ctor{}, std::forward<decltype(args)>(args)...);
        };
    }

private:
    template<class TInitialization, class... TCtor, class TInjector, class... TArgs>
    auto create(const TInjector& injector, const di::aux::pair<TInitialization, di::aux::type_list<TCtor...>>& ctor, TArgs&&... args) const {
        using seq = std::make_index_sequence<sizeof...(TCtor)>;
        return create_helper(seq{}, injector, ctor, std::forward<decltype(args)>(args)...);
    }

    template<class TInitialization, class... TCtor, std::size_t... Ns, class TInjector, class... TArgs>
    auto create_helper(const std::index_sequence<Ns...>&, const TInjector& injector, const di::aux::pair<TInitialization, di::aux::type_list<TCtor...>>&, TArgs&&... args) const {
        return std::make_unique<T>(create_impl<Ns>(injector, di::aux::type<TCtor>{}, std::forward<TArgs>(args)...)...);
    }

    template<std::size_t, class TInjector, class TArg>
    decltype(auto) create_impl(const TInjector& injector, const di::aux::type<TArg>&, ...) const {
        return injector.template create<TArg>();
    }

    template<std::size_t N, class TInjector, class TArg, class... TArgs>
    decltype(auto) create_impl(const TInjector&, const di::aux::type<di::type_traits::named<di::no_name, TArg>>&, TArgs&&... args) const {
        constexpr auto value = get_assisted_ctor_nr(N, typename di::type_traits::ctor_traits<T>::type{});
        return get<TArg>(std::integral_constant<std::size_t, value>{}, args...);
    }

    template<class TInitialization, class... TCtor>
    static constexpr auto get_assisted_ctor_nr(std::size_t N, const di::aux::pair<TInitialization, di::aux::type_list<TCtor...>>&) {
        return sum(0, N, is_assisted(di::aux::type<TCtor>{})...);
    }

    template<class... Args>
    static constexpr auto sum(std::size_t index, std::size_t N, bool I, Args... V) {
        if (index < N) {
            return static_cast<int>(I) + sum(index + 1, N, V...);
        }
        return static_cast<int>(I);
    }

    static constexpr auto sum(std::size_t, std::size_t) {
        return 0;
    }

    template<class TArg>
    static constexpr auto is_assisted(const di::aux::type<TArg>&) {
        return false;
    }

    template<class TArg>
    static constexpr auto is_assisted(const di::aux::type<di::type_traits::named<di::no_name, TArg>>&) {
        return true;
    }

    template<class TCtor, std::size_t N, class TArg, class... TArgs, class = std::enable_if_t<(N > 1)>>
    decltype(auto) get(const std::integral_constant<std::size_t, N>&, TArg&&, TArgs&&... args) const {
        return get<TCtor>(std::integral_constant<std::size_t, N - 1>{}, args...);
    }

    template<class TCtor, class TArg, class... TArgs>
    decltype(auto) get(const std::integral_constant<std::size_t, 1>&, TArg&& arg, TArgs&&...) const {
        return arg;
    }

    template<class TCtor>
    auto get(const std::integral_constant<std::size_t, 0>&) const {
        return TCtor{};
    }
};

//->

struct example {
    /*<<define `example` constructor using `assisted` as factory creation parameters>>*/
    BOOST_DI_INJECT(example
                  , (named = assisted) int date /*from factory*/
                  , std::unique_ptr<interface> up /*from injector*/
                  , (named = assisted) double factor /*from factory*/
                  , int i /*from injector*/) {
        assert(i == 87);
        assert(dynamic_cast<implementation*>(up.get()));
        assert(date == 42);
        assert(factor == 123.0);
    }
};

int main() {
    /*<<factory declaration using std function>>*/
    using example_factory = std::function<std::unique_ptr<example>(int, double)>;

    auto injector = di::make_injector(
        di::bind<int>.to(87)
      , di::bind<interface, implementation>
        /*<<bind `example_factory` to factory creating `example`>>*/
      , di::bind<example_factory>.to(factory<example>{})
    );

    auto f = injector.create<example_factory>();
    f(42, 123.0);
}

//]

