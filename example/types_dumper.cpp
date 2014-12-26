//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[types_dumper
//<-
#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>
#include <boost/units/detail/utility.hpp>
//->
#include <boost/di.hpp>

//<-
namespace utils = boost::units::detail;

auto int_1 = []{ return "first int"; };
auto int_2 = []{ return "second int"; };

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };
struct c1 { c1(std::shared_ptr<i0>, int) { } };
struct c2 { BOOST_DI_INJECT(c2, (named = int_1) int, (named = int_2) int, char) { } };
struct c3 { c3(std::shared_ptr<c1>, std::shared_ptr<c2>) { } };
//->
namespace di = boost::di;

class local_config : public di::config {
public:
    auto policies() const noexcept {
        return di::make_policies(
            [&](auto type, auto dependency, auto... ctor) {
                using T = decltype(type);
                using arg = typename T::type;
                using name = typename T::name;
                using given = typename decltype(dependency)::given;

                auto tab = v[i - 1];
                while (tab--) {
                    std::clog << "    ";
                }

                std::clog << "(" << utils::demangle(typeid(arg).name())
                          << "[" << name{}() << "]"
                          << " -> " << utils::demangle(typeid(given).name())
                          << ")" << std::endl;

                auto ctor_size = sizeof...(ctor);
                while (ctor_size--) {
                    v.insert((v.begin()+i), v[i-1]+1);
                }
                ++i;
            }
        );
    }

    mutable std::vector<int> v = { 0 };
    mutable int i = 1;
};

int main() {
    /*<<define injector>>*/
    auto injector = di::make_injector<local_config>(
        di::bind<i0, c0>
    );

    /*<<iterate through created objects with `types_dumper`>>*/
    injector.create<c3>();

    /*<< output [pre
        void : (c3[no_name] -> c3)
        c3 : (std::shared_ptr<c1>[no_name] -> c1)
        std::shared_ptr<c1> : (std::shared_ptr<i0>[no_name] -> c0)
        std::shared_ptr<c1> : (int[no_name] -> int)
        c3 : (std::shared_ptr<c2>[no_name] -> c2)
        std::shared_ptr<c2> : (int[first int] -> int)
        std::shared_ptr<c2> : (int[second int] -> int)
        std::shared_ptr<c2> : (char[no_name] -> char)
    ]>>*/
    return 0;
}

//]

