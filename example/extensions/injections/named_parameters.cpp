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

namespace di = boost::di;

//<-
struct interface { virtual ~interface() = default; virtual void dummy() = 0; };
struct implementation : interface { void dummy() override { } };

long constexpr const_strlen(char const *input, long i = 0) {
    return !*input ? i : const_strlen(input + 1, i + 1);
}

long constexpr const_hash(char const *input, long m = 0, long i = 0) {
    return *input && i < m ? static_cast<long>(*input) + 33 * const_hash(input + 1, m, i+1) : 5381;
}

auto constexpr const_type(long N, char const *input) {
    auto i = 0;
    auto j = 0;
    auto e = 0;
    auto n = false;
    for (i = 0; i < const_strlen(input); ++i) {
        if (input[i] == ' ') {
            n = true;
        } else if (n == true && input[i] != ' ') {
            e = i;
            n = false;
        } else if (input[i] == ',') {
            j = i - e;
            break;
        }
    }
    if (!j) {
        j = i - e;
    }
    return std::make_pair(N + i + 1, const_hash(input + e, j));
}

template<long N, class C, class T>
struct get_type {
    using type = std::integral_constant<long, const_type(N, &C::boost_di_inject_str__[N]).second>;
    using next = std::integral_constant<long, const_type(N, &C::boost_di_inject_str__[N]).first>;
};

template<long, class, class...>
struct named_types_impl;

template<long N, class R, class T>
struct named_types_impl<N, R, T> {
    using type = di::aux::type_list<typename get_type<N, R, T>::type>;
};

template<long N, class R, class T, class... TArgs>
struct named_types_impl<N, R, T, TArgs...> {
    using type = di::aux::join_t<
        di::aux::type_list<typename get_type<N, R, T>::type>
      , typename named_types_impl<get_type<N, R, T>::next::value, R, TArgs...>::type
    >;
};

template<class>
struct named_types;

template<class R, class... TArgs>
struct named_types<R(TArgs...)>
    : named_types_impl<0, R, TArgs...>
{ };

template<class T>
using named_types_t = typename named_types<T>::type;

#define $inject(type, ...) \
    static type boost_di_inject_ctor__(__VA_ARGS__); \
    static constexpr auto boost_di_inject_str__ = #__VA_ARGS__; \
    using boost_di_inject__ = ::boost::di::detail::combine_t< \
        ::boost::di::aux::function_traits_t<decltype(boost_di_inject_ctor__)> \
      , named_types_t<decltype(boost_di_inject_ctor__)> \
    >; \
    type(__VA_ARGS__)

#define $(name) \
    std::integral_constant<long, const_hash(#name, const_strlen(#name))>{}
//->

struct example {
    /*<<inject constructor using automatic named parameters>>*/
    $inject(example, int i, std::unique_ptr<interface> up, int value) {
        assert(i == 42);
        assert(dynamic_cast<implementation*>(up.get()));
        assert(value == 87);
    }
};

int main() {
   auto injector = di::make_injector(
        /*<<bind named parameters>>*/
        di::bind<int>().named($(i)).to(42)
      , di::bind<interface>().named($(up)).to<implementation>()
      , di::bind<int>().named($(value)).to(87)
    );

    injector.create<example>();
}

//]

