//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[named_parameters
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

#include <iostream>
#include <boost/units/detail/utility.hpp>

#if !defined(__cpp_constexpr)
    int main() { }
#else

namespace di = boost::di;

//<-
struct interface { virtual ~interface() = default; virtual void dummy() = 0; };
struct implementation : interface { void dummy() override { } };

template<char...> struct chars { };
struct pair { long begin; long end; };

template<class T, T... Chars>
constexpr auto operator""_s() {
    return std::integral_constant<long, const_hash(chars<Chars...>{}, sizeof...(Chars) + 1)>{};
}

long constexpr const_hash(char const *input, long m = 0, long i = 0) {
    return *input && i < m ? static_cast<long>(*input) + 33 * const_hash(input + 1, m, i + 1) : 5381;
}

template<char C, char... Chars>
long constexpr const_hash(const chars<C, Chars...>&, long m = 0, long i = 0) {
    return C && i < m ? static_cast<long>(C) + 33 * const_hash(chars<Chars...>{}, m, i + 1) : 5381;
}

long constexpr const_hash(const chars<>&, long m = 0, long i = 0) {
    return 5381;
}

constexpr pair get_name_impl(const char* input, int begin, int n = 0, int quote = 0) {
    return !*input || *input == ',' ? pair{0, 0} : (
            quote == 2 ? pair{begin + 1, n} : (
                quote == 1 && *input == '"' ? get_name_impl(input + 1, begin, n, quote + 1) :
                quote == 0 && *input == '"' ? get_name_impl(input + 1, begin, n, quote + 1) :
                get_name_impl(input + 1, quote == 0 ? begin + 1 : begin , quote == 1 ? n + 1 : n, quote)
    ));
}

constexpr pair get_name(const char* input, int N, int Q = 0, int i = 0) {
    return
        Q == N ? get_name_impl(input + 1, i + 1) : (
            *input ? (
                *input == ',' ?
                    get_name(input + 1, N, Q + 1, i + 1) :
                    get_name(input + 1, N, Q, i + 1)
            ) : pair{0, 0}
        );
}

template<class T, class TArg, int N>
struct parse {
    static constexpr auto name = get_name(T::boost_di_inject_str__, N - 1);
    using type = std::conditional_t<name.begin == name.end
      , TArg
      , di::named<std::integral_constant<long, const_hash(&T::boost_di_inject_str__[name.begin], name.end)>, TArg>
    >;
};

template<class, class, class...>
struct args_impl;

template<class T, class... TArgs, int... Ns>
struct args_impl<T, di::aux::index_sequence<Ns...>, TArgs...> {
    using type = di::aux::type_list<typename parse<T, TArgs, Ns>::type...>;
};

template<class>
struct args;

template<class R, class... TArgs>
struct args<R(TArgs...)> {
    using type = typename args_impl<R, di::aux::make_index_sequence<sizeof...(TArgs)>, TArgs...>::type;
};

template<class T>
using args_t = typename args<T>::type;

#define $inject(type, ...) \
    static constexpr auto boost_di_inject_str__ = #__VA_ARGS__; \
    static type boost_di_inject_ctor__(__VA_ARGS__); \
    using boost_di_inject__ = args_t<decltype(boost_di_inject_ctor__)>; \
    type(__VA_ARGS__)

//->

struct example {
    /*<<inject constructor using automatic named parameters>>*/
    $inject(example, int i, std::unique_ptr<interface> up, [[named("my_value")]] int value) {
        assert(i == 42);
        assert(dynamic_cast<implementation*>(up.get()));
        assert(value == 87);
    }
};

int main() {
   auto injector = di::make_injector(
        /*<<bind named parameters>>*/
        di::bind<int>.to(42)
      , di::bind<interface>.to<implementation>()
      , di::bind<int>.named("my_value"_s).to(87)
    );

    injector.create<example>();
}

//]

#endif

