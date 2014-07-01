//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[text_visitor
//````C++98/03/11/14```
//<-
#include <iostream>
#include <memory>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/units/detail/utility.hpp>
#include "common/data.hpp"
//->
#include <boost/di.hpp>

//<-
namespace mpl = boost::mpl;
namespace utils = boost::units::detail;
//->
namespace di = boost::di;

/*<<Definition of text visitor>>*/
class text_visitor
{
public:
    /*<<Definition of the visitor call operator requirement>>*/
    template<typename T>
    void operator()(const T&) const {
        int size = mpl::size<typename T::call_stack>::value;
        while(--size) {
            std::cout << "\t";
        }
        std::cout << utils::demangle(typeid(typename T::type).name()) << std::endl;
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

