//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[text_visitor
//<-
#include <iostream>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/units/detail/utility.hpp>
//->
#include <boost/di.hpp>

//<-
namespace mpl = boost::mpl;
namespace utils = boost::units::detail;

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };
struct c1 { c1(boost::shared_ptr<i0>, int) { } };
struct c2 { c2(int, double, char) { } };
struct c3 { c3(boost::shared_ptr<c1>, boost::shared_ptr<c2>) { } };
//->
namespace di = boost::di;

/*<<Definition of text visitor>>*/
class text_visitor {
public:
    /*<<Definition of the visitor call operator requirement>>*/
    template<typename T>
    void operator()(const T&) const {
        auto size = mpl::size<typename T::call_stack>::value;
        while(--size) {
            std::clog << "\t";
        }
        std::clog << utils::demangle(typeid(typename T::type).name()) << std::endl;
    }
};

int main() {
    /*<<define injector>>*/
    di::injector<c0> injector; // or auto injector = di::make_injector<di::deduce<c0>>();

    /*<<iterate through created objects with `text_visitor`>>*/
    injector.visit<c3>(text_visitor());

    /*<< output [pre
    c3
        boost::shared_ptr<c1>
            boost::shared_ptr<i0>
            int
        boost::shared_ptr<c2>
            int
            double
            char
    ]>>*/
    return 0;
}

//]

