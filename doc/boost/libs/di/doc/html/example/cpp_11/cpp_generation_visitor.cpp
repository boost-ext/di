//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[cpp_generation_visitor
//````C++98/03/11/14```
//<-
#include <iostream>
#include <memory>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/units/detail/utility.hpp>
//->
#include <boost/di.hpp>

//<-
namespace mpl = boost::mpl;
namespace utils = boost::units::detail;
//->
namespace di = boost::di;

struct i { virtual ~i() { }; };
struct impl : i { };
struct c1 { c1(boost::shared_ptr<i>, int) { } };
struct c2 { c2(int, double, char) { } };
struct example { example(boost::shared_ptr<c1>, boost::shared_ptr<c2>) { } };

/*<<Definition of text visitor>>*/
class cpp_generation_visitor
{
public:
    /*<<Definition of the visitor call operator requirement>>*/
    template<typename T>
    void operator()(const T&) const {
        auto call_stack = mpl::size<typename T::call_stack>::value;

        if (call_stack == 1) {
            std::clog << utils::demangle(typeid(typename T::type).name()) << " app(";
        } else {
            std::clog << utils::demangle(typeid(typename T::type).name());
        }
    }
};

int main() {
    /*<<define injector>>*/
    auto injector = di::make_injector(di::deduce<impl>());

    /*<<iterate through created objects with `cpp_generation_visitor`>>*/
    injector.visit<example>(cpp_generation_visitor());

    /*<< output [pre
    ]>>*/
    return 0;
}

//]

