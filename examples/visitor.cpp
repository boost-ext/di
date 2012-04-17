//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/mpl/size.hpp>
#include <di/di.hpp>
#include "common/utility.hpp"
#include "common/data.hpp"

namespace base    = di::front::base;
namespace utility = di::utility;

class Printvisitor
{
public:
    template<typename T> void operator()() const
    {
        int size = boost::mpl::size<typename T::context>::value;
        while(--size) {
            std::cout << "\t";
        }
        std::cout << demangle(typeid(typename T::type).name()) << std::endl;
    }
};

struct visitormodule : base::module
    <
        base::singletons <
            CIf0
        >
    >
{ };

int main()
{
    utility::Injector<visitormodule> injector;
    injector.visit<C8>(Printvisitor());

    return 0;
}

