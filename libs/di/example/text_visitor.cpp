//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/mpl/size.hpp>
#include <boost/units/detail/utility.hpp>
#include <boost/di.hpp>
#include "data.hpp"

namespace mpl   = boost::mpl;
namespace utils = boost::units::detail;
namespace di    = boost::di;

class print_visitor
{
public:
    template<typename T>
    void operator()() const {
        int size = mpl::size<typename T::context>::value;
        while(--size) {
            std::cout << "\t";
        }
        std::cout << utils::demangle(typeid(typename T::type).name()) << std::endl;
    }
};

int main()
{
    typedef di::generic_module<
        di::singletons<
            c0if0
        >
    > visitor_module;

    di::injector<visitor_module> injector;
    injector.visit<c8>(print_visitor());

    return 0;
}

