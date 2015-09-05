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

int constexpr strlen(char const *input, int i = 0) {
    return !*input ? i : strlen(input + 1, i + 1);
}

unsigned constexpr const_hash(char const *input, int m = 0, int i = 0) {
    //std::cout << *input << " " << i << std::endl;
    return *input && i < m ? static_cast<long>(*input) + 33 * const_hash(input + 1, m, i+1) : 5381;
}

unsigned constexpr get_type(char const *input) {
    int i = 0;
    int j = 0;
    int e = 0;
    bool n = false;
    for (i = 0; i < strlen(input); ++i) {
        //std::cout << input[i] << std::endl;
        if (input[i] == ' ') {
            n = true;
    //std::cout << "true" << i << std::endl;
        } else if (n == true && input[i] != ' ') {
            e = i;
            n = false;
    //std::cout << "name" << i << std::endl;
        } else if (input[i] == ',') {
    //std::cout << "comma" << i << std::endl;
            auto q = i - e;
            j = q;
            break;
        }
    }

    //std::cout << "i=" << e << std::endl;
    return const_hash(input + e, j);
}

template<class>
struct function_traits;

template<class C, class T>
struct get_tn {
    using type = di::aux::type_list<std::integral_constant<long, get_type(C::str)>, T>;
};

template<class R, class... TArgs>
struct function_traits<R(TArgs...)> {
    using type = di::aux::type_list<typename get_tn<R, TArgs>::type...>;
};

template<class T>
using function_traits_t = typename function_traits<T>::type;

#define INJECT(type, ...) \
    static constexpr auto str = #__VA_ARGS__; \
    static void ctor(__VA_ARGS__); \
    using f = function_traits_t<decltype(ctor<di::_>)>; \
    type(__VA_ARGS__)
//->

struct example {
    /*<<inject constructor using automatic named parameters>>*/
    INJECT(example, int i, std::unique_ptr<interface> up, int value) {
        assert(i == 42);
        assert(dynamic_cast<implementation*>(up.get()));
        assert(value == 87);
    }
};

int main() {
/*    auto injector = di::make_injector(*/
        //[><<bind named parameters>><]
        //di::bind<int>().named(_N(i)).to(42)
      //, di::bind<interface>().named(_N(up)).to<implementation>()
      //, di::bind<int>().named(_N(value)).to(87)
    //);

    /*injector.create<example>();*/
}

//]

