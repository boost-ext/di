//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[uml_dumper
//<-
#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
//->
#include <boost/di.hpp>

auto int_1 = []{ return "first int"; };
auto int_2 = []{ return "second int"; };

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };
struct c1 { c1(std::shared_ptr<i0>, int) { } };
struct c2 { BOOST_DI_INJECT(c2, (named = int_1) int, (named = int_2) int, char) { } };
struct c3 { c3(std::shared_ptr<c1>, std::shared_ptr<c2>) { } };

namespace di = boost::di;

/**
 * http://plantuml.sourceforge.net/objects.html
 * ./uml_dumper | java -jar plantuml.jar -p > uml_dumper.png
 */
class uml_dumper : public di::config {
public:
    uml_dumper() {
        std::cout << "@startuml uml_dumper.png" << std::endl;
    }

    ~uml_dumper() {
        std::cout << "@enduml" << std::endl;
    }

    auto policies() noexcept {
        /*<<define `uml dumper` directly in policies configuration>>*/
        return di::make_policies(
            [&](auto type, auto dependency, auto... ctor) {
                using T = decltype(type);
                using name = typename T::name;
                using given = typename decltype(dependency)::given;

                auto root = v[i - 1];
                if (root != &typeid(nullptr)) {
                    std::cout << "\"" <<root->name() << "\" .. \"" << typeid(given).name()
                              << (name{}() ? std::string(" [") + name{}() + std::string("]") : "")
                              << "\"" << std::endl;
                }

                auto ctor_size = sizeof...(ctor);
                while (ctor_size--) {
                    v.insert((v.begin()+i), &typeid(given));
                }
                ++i;
            }
        );
    }

private:
    std::vector<const std::type_info*> v = { &typeid(nullptr) };
    int i = 1;
};

int main() {
    /*<<define injector>>*/
    auto injector = di::make_injector<uml_dumper>(
        di::bind<i0, c0>
    );

    /*<<iterate through created objects with `types_dumper`>>*/
    injector.create<c3>();

    /*<<output [@images/uml_dumper.png [$images/uml_dumper.png [width 75%] [height 75%] ]]>>*/
    return 0;
}

//]

