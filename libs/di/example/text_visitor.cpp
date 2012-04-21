//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/mpl/size.hpp>
#include <boost/di.hpp>
#include "data.hpp"

namespace mpl = boost::mpl;
namespace di  = boost::di;

class print_visitor
{
public:
    template<typename T>
    void operator()() const {
        int size = mpl::size<typename T::context>::value;
        while(--size) {
            std::cout << "\t";
        }
        std::cout << typeid(typename T::type).name() << std::endl;
    }
};

typedef di::generic_module<
    di::singletons<
        c0if0
    >
> visitor_module;

int main()
{
    di::injector<visitor_module> injector;
    injector.visit<c8>(print_visitor());

    return 0;
}

