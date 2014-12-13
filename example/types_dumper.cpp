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
#include <boost/units/detail/utility.hpp>
//->
#include <boost/di.hpp>

//<-
namespace utils = boost::units::detail;

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };
struct c1 { c1(std::shared_ptr<i0>, int) { } };
struct c2 { c2(int, double, char) { } };
struct c3 { c3(std::shared_ptr<c1>, std::shared_ptr<c2>) { } };
//->
namespace di = boost::di;

/*<<Definition of text visitor>>*/
class types_dumper {
public:
    /*<<Definition of the visitor call operator requirement>>*/
    template<typename T>
    void operator()(const T&) const {
        //auto size = mpl::size<typename T::call_stack>::value;
        auto size = 1;
        while(--size) {
            std::clog << "\t";
        }
        std::clog << utils::demangle(typeid(typename T::type).name()) << std::endl;
    }
};

class local_config : public di::config {
public:
    auto policies() const noexcept {
        return di::make_policies(types_dumper{});
    }
};

int main() {
    /*<<define injector>>*/
    auto injector = di::make_injector<local_config>(
        di::bind<i0, c0>
    );

    /*<<iterate through created objects with `types_dumper`>>*/
    injector.create<c3>();

    /*<< output [pre
    c3
        std::shared_ptr<c1>
            std::shared_ptr<i0>
            int
        std::shared_ptr<c2>
            int
            double
            char
    ]>>*/
    return 0;
}

//]

